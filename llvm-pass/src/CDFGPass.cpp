#include "llvm/Pass.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/CFGPrinter.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/GraphWriter.h"
#include "llvm/Analysis/MemoryDependenceAnalysis.h"

#include "llvm/Transforms/Utils.h"

#include "llvm/Transforms/Scalar.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/CaptureTracking.h"
#include "llvm/Analysis/GlobalsModRef.h"
#include "llvm/Analysis/MemoryBuiltins.h"
#include "llvm/Analysis/MemoryDependenceAnalysis.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/Support/CommandLine.h"

#include "llvm/ADT/GraphTraits.h"

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/Passes.h"

#include "llvm/Analysis/ScalarEvolution.h"
//#include "llvm/Analysis/ScalarEvolutionExpander.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"

#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"

#include "llvm/IR/Attributes.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <set>

#include "llvm_cdfg.h"


using namespace llvm;

// mapping unit : loop
typedef struct
{
	bool isInnerLoop = false;
	Loop *lp;
	std::set<BasicBlock *> allBlocks;
	std::set<std::pair<BasicBlock *, BasicBlock *>> entryBlocks;
	std::set<std::pair<BasicBlock *, BasicBlock *>> exitBlocks;
} MappingUnit;


// parse function annotation, attribute key-values
// sizeAttrMap[attrKey] = attrValue
void ParseSizeAttr(Function &F, std::map<std::string, int>& sizeAttrMap)
{
	auto global_annos = F.getParent()->getNamedGlobal("llvm.global.annotations");
	if (global_annos)
	{
		auto a = cast<ConstantArray>(global_annos->getOperand(0));
		for (int i = 0; i < a->getNumOperands(); i++)
		{
			auto e = cast<ConstantStruct>(a->getOperand(i));
			if (auto fn = dyn_cast<Function>(e->getOperand(0)->getOperand(0)))
			{
				auto anno = cast<ConstantDataArray>(cast<GlobalVariable>(e->getOperand(1)->getOperand(0))->getOperand(0))->getAsCString();
				fn->addFnAttr("size", anno); // <-- add function annotation here
			}
		}
	}
	if (F.hasFnAttribute("size"))
	{
		Attribute attr = F.getFnAttribute("size");
		outs() << "Size attribute : " << attr.getValueAsString() << "\n";
		StringRef sizeAttrStr = attr.getValueAsString();
		SmallVector<StringRef, 8> sizeArr;
		sizeAttrStr.split(sizeArr, ',');
		for (int i = 0; i < sizeArr.size(); ++i)
		{
			std::pair<StringRef, StringRef> splitDuple = sizeArr[i].split(':');
			uint32_t size;
			splitDuple.second.getAsInteger(10, size);
			outs() << "ParseAttr:: name:" << splitDuple.first << ",size:" << size << "\n";
			sizeAttrMap[splitDuple.first.str()] = size;
		}
	}
}


// Depth-first search for the recursively successive basic blocks except the back-edge BBs
void dfsForSuccBBs(SmallVector<std::pair<const BasicBlock *, const BasicBlock *>, 1> BackEdgeBBPs,
		std::map<const BasicBlock *, std::set<const BasicBlock *>> &SuccBBsMap,
		const BasicBlock *currBB, const BasicBlock *startBB){
	for (auto iter = succ_begin(currBB); iter != succ_end(currBB); ++iter){
		const BasicBlock *succ = *iter;
        // skip back-edge BB
		std::pair<const BasicBlock *, const BasicBlock *> bbPair(currBB, succ);
		if (std::find(BackEdgeBBPs.begin(), BackEdgeBBPs.end(), bbPair) != BackEdgeBBPs.end()){//if find back-egde
			continue;
		}
		SuccBBsMap[startBB].emplace(succ);
		dfsForSuccBBs(BackEdgeBBPs, SuccBBsMap, succ, startBB);
	}
}


// get the recursively successive basic blocks except the back-edge BBs for each BB  
// Including self-BB
void getSuccBBsMap(Function &F, SmallVector<std::pair<const BasicBlock *, const BasicBlock *>, 1> &BackEdgeBBPs, 
        std::map<const BasicBlock *, std::set<const BasicBlock *>> &SuccBBsMap){
    for(auto &BB : F){
        const BasicBlock *BBPtr = &BB;
        SuccBBsMap[BBPtr].emplace(BBPtr);
        dfsForSuccBBs(BackEdgeBBPs, SuccBBsMap, BBPtr, BBPtr);
    }
}


void printSuccBBsMap(std::map<const BasicBlock *, std::set<const BasicBlock *>> &SuccBBsMap){
    outs() << "###### SuccBBsMap ######\n";
	for (auto &elem : SuccBBsMap){
		outs() << "SuccBBs(" << elem.first->getName().str() << ") : ";
		for (auto &succ : elem.second){
			outs() << succ->getName().str() << ", ";
		}
		outs() << "\n";
	}
}


// // get BBs not in the loops
// void getNonLoopBBs(Function &F, std::vector<Loop *> &loops, std::set<BasicBlock *> &nonloopBBs){
// 	nonloopBBs.clear();
//     for (BasicBlock &BB : F){
// 		BasicBlock *BBPtr = &BB;
// 		nonloopBBs.emplace(BBPtr);
// 	}
// 	for (Loop *lp : loops){
// 		for (BasicBlock *BB : lp->getBlocks()){
// 			nonloopBBs.erase(BB);
// 		}
// 	}
// }

// get inner-most loops (mapping units) according to LoopInfo
void getInnerMostLoops(std::map<std::string, MappingUnit> &mappingUnitMap, std::map<Loop *, std::string> &loopNames, 
    std::vector<Loop *> loops, std::string lnstr)
{
	for (int i = 0; i < loops.size(); ++i)
	{
        auto& loop = loops[i];
        std::string name = lnstr + "_" + std::to_string(i);
		loopNames[loop] = name;

		if (loop->getSubLoops().size() == 0){
			// innerMostLoops.push_back(loop);
            std::string loopName = "INNERMOST_" + name;
			mappingUnitMap[loopName].isInnerLoop = true;
			mappingUnitMap[loopName].lp = loop;
            mappingUnitMap[loopName].allBlocks.insert(loop->getBlocks().begin(), loop->getBlocks().end());
            SmallVector<std::pair<BasicBlock *, BasicBlock *>, 8> exitEdges;
            loop->getExitEdges(exitEdges);
            mappingUnitMap[loopName].exitBlocks.insert(exitEdges.begin(), exitEdges.end());
            mappingUnitMap[loopName].entryBlocks.insert(std::make_pair(loop->getLoopPredecessor(), loop->getHeader()));
			// for (BasicBlock *BB : loop->getBlocks()){
			// 	// loopsExclusieBasicBlockMap[loops[i]].push_back(BB);
			// 	// mappingUnitMap[loopName].allBlocks.insert(BB);
			// 	BB2MUnitMap[BB] = loopName;
			// }
		}else{
			getInnerMostLoops(mappingUnitMap, loopNames, loop->getSubLoops(), name);

			// for (BasicBlock *BB : loop->getBlocks()){
			// 	bool BBfound = false;
			// 	for (std::pair<Loop *, std::vector<BasicBlock *>> pair : loopsExclusieBasicBlockMap)
			// 	{
			// 		if (std::find(pair.second.begin(), pair.second.end(), BB) != pair.second.end())
			// 		{
			// 			BBfound = true;
			// 			break;
			// 		}
			// 	}
			// 	if (!BBfound)
			// 	{
			// 		loopsExclusieBasicBlockMap[loops[i]].push_back(BB);
			// 	}
			// }
		}
	}
}


void printMappingUnitMap(std::map<std::string, MappingUnit> &mappingUnitMap)
{
	outs() << "###### MappingUnitMap ###### \n";
	for (std::pair<std::string, MappingUnit> pair : mappingUnitMap)
	{
		outs() << pair.first << " :: ";
		for (BasicBlock *bb : pair.second.allBlocks)
		{
			outs() << bb->getName() << ", ";
		}
		outs() << "| entry = ";
		for (std::pair<BasicBlock *, BasicBlock *> bbPair : pair.second.entryBlocks)
		{
			outs() << bbPair.first->getName() << " to " << bbPair.second->getName() << ", ";
		}
		outs() << "| exit = ";
		for (std::pair<BasicBlock *, BasicBlock *> bbPair : pair.second.exitBlocks)
		{
			outs() << bbPair.first->getName() << " to " << bbPair.second->getName() << ", ";
		}
		outs() << "\n";
	}
}

// get target loop name (mapping unit name) using the token function "please_map_me"
std::string getMappingUnitNameUsingPleasemapme(Function &F, std::map<std::string, MappingUnit> &mappingUnitMap)
{
	BasicBlock *MUBB;
	Instruction *checker_ins = NULL;
	for (auto &BB : F){
		// BB.dump();
		for (auto &I : BB){
			if (CallInst *CI = dyn_cast<CallInst>(&I)){
				std::string op_str;
				raw_string_ostream rs(op_str);
				CI->print(rs);
				outs() << "op : " << rs.str() << "\n";
				if (op_str.find("please_map_me") != std::string::npos){
					outs() << "token found in BB = " << BB.getName() << "\n";
					MUBB = &BB;
                    checker_ins = CI;
				}
			}
		}
	}
	assert(MUBB);
	assert(checker_ins);
	checker_ins->eraseFromParent();

	for (auto &elem : mappingUnitMap){
		errs() << "loop name: "<< elem.first << "\n";
		if (elem.second.lp->contains(MUBB)){
			return elem.first;
		}
	}
	assert(false);
}

// get target loop name (mapping unit name) using the token function "loop_begin()" & "loop_end()"
std::vector<std::map<int, Loop*>> getMappingUnitNameUsingLoopMark(Function &F, std::map<std::string, MappingUnit> &mappingUnitMap, std::map<const llvm::BasicBlock *, std::set<const llvm::BasicBlock *>> SuccBBsMap)
{
	BasicBlock *EntryBB, *ExitBB;
	Instruction *LoopStart = NULL;
	Instruction *LoopEnd = NULL;
	std::vector<std::map<int, Loop*>> LoopVec;
	std::map<int, Loop*> nestLoop;
	std::map<int, Loop*> finalnestloop;
	for (auto &BB : F){
		// BB.dump();
		for (auto &I : BB){
			if (CallInst *CI = dyn_cast<CallInst>(&I)){
				std::string op_str;
				raw_string_ostream rs(op_str);
				CI->print(rs);
				outs() << "op : " << rs.str() << "\n";
				if (op_str.find("loop_begin") != std::string::npos){
					outs() << "EntryBB = " << BB.getName() << "\n";
					EntryBB = &BB;
                    LoopStart = CI;
				}
				if (op_str.find("loop_end") != std::string::npos){
					outs() << "LoopEnd = " << BB.getName() << "\n";
					ExitBB = &BB;
                    LoopEnd = CI;
				}
			}
		}
	}
	if(LoopStart == NULL && LoopEnd == NULL){
		LoopVec.clear();
		nestLoop.clear();
	}else if(LoopStart != NULL && LoopEnd != NULL){
		std::set<BasicBlock*> internalBBs;
		for(auto temBB : SuccBBsMap[EntryBB]){
			if(!SuccBBsMap[ExitBB].count(temBB)){
				internalBBs.insert((BasicBlock*)temBB);
			}
		}
		errs() << "internalBBs: ";
		for(auto iter:internalBBs){
			errs()<< iter->getName() <<"; ";
		}
		errs() << "\n";
		LoopStart->eraseFromParent();
		LoopEnd->eraseFromParent();
		for (auto &elem : mappingUnitMap){
			errs() << "loop name: "<< elem.first << "\n";
			nestLoop.clear();
			finalnestloop.clear();
			auto tem = elem.second.lp;
			bool allin = true;
			int level = 0;
			while(tem != nullptr){
				nestLoop[level] = tem;
				tem = tem->getParentLoop();
				level++; 
			}
			//auto loopBBs = nestLoop[level-1]->getBlocks();
			int targetlevel = -1;
			for(int i = 0; i < level; i++){
				auto loopBBs = nestLoop[i]->getBlocks();
				bool hereEntryBB = false;
				for(auto BB : loopBBs){
					if(BB == EntryBB){
						assert(i!=0);
						targetlevel = i -1;
						hereEntryBB = true;
					}
				}
				if(hereEntryBB)
					break;
				finalnestloop[i] = nestLoop[i];
			}
			if(targetlevel=-1)
				targetlevel = level-1;
			auto loopBBs = finalnestloop[targetlevel]->getBlocks();
			for(auto BB : loopBBs){
				if(!internalBBs.count(BB)){
					allin = false;
					break;
				}
			}
			if (allin){
				LoopVec.push_back(finalnestloop);
			}
		}
	}else{
		assert(false && "illegal loop mark");
	}
	return LoopVec;
}

//extract information of Load/Store in this kernel
void extractLSInfo(LLVMCDFG* CDFG, int kernel, std::map<int, std::pair<std::set<LLVMCDFGNode*>, std::set<LLVMCDFGNode*>>>* LSTable){
	for(auto elem : CDFG->nodes()){
		auto node = elem.second;
		Instruction* ins = node->instruction();
		GetElementPtrInst* GEPins = NULL;
		if(ins != NULL){
			if(dyn_cast<LoadInst>(ins)){
				(*LSTable)[kernel].first.insert(node);
			}
			else if(dyn_cast<StoreInst>(ins)){
				(*LSTable)[kernel].second.insert(node);
			}
		}
		///TODO: maybe use other name as symbol of regIO(?)
		else{
			if(node->finalInstruction() == "INPUT"){
				(*LSTable)[kernel].first.insert(node);
			}
			else if(node->finalInstruction() == "OUTPUT"){
				(*LSTable)[kernel].second.insert(node);
			}
		}
	}
}

//merge tasks and schedule accroding to access dependence
void scheduleTasks(std::map<int, LLVMCDFG*> CDFGs, std::map<int, std::pair<std::set<LLVMCDFGNode*>, std::set<LLVMCDFGNode*>>> LSTable){
	outs() <<">>>>>> Merge tasks and Schedule accroding to access dependence <<<<<<\n";
	std::map<int, std::map<Value*, std::pair<std::set<LLVMCDFGNode*>, std::set<LLVMCDFGNode*>>>> LSaddrTable;
	std::map<int, std::pair<std::set<Value*>, std::set<Value*>>> LSaddrSets;
	for(int i = 0; i < LSTable.size(); i++){
		outs() << "-----kernel_" << i << "-----\n";
		outs() << "\tload values: ";
		std::set<LLVMCDFGNode*> LoadSet = LSTable[i].first;
		for(auto LoadNode : LoadSet){
			auto addrValue = LoadNode->getLSaddress();
			assert(addrValue != NULL);
			outs() << addrValue->getName().str() << ", ";
			LSaddrTable[i][addrValue].first.insert(LoadNode);
			LSaddrSets[i].first.insert(addrValue);
		}
		outs() << "\n\tstore values: ";
		std::set<LLVMCDFGNode*> StoreSet = LSTable[i].second;
		for(auto StoreNode : StoreSet){
			auto addrValue = StoreNode->getLSaddress();
			assert(addrValue != NULL);
			outs() << addrValue->getName().str() << ", ";
			LSaddrTable[i][addrValue].second.insert(StoreNode);
			LSaddrSets[i].second.insert(addrValue);
		}
		outs() << "\n";
	}
	std::ofstream ofs;
	ofs.open("whole_DFG.dot");
	ofs << "digraph g{\n";
	for(int i = 0; i < CDFGs.size(); i++){
		CDFGs[i]->printAsSubTask(ofs, i);
	}
	std::set<std::pair<LLVMCDFGNode*, LLVMCDFGNode*>> nodepair;
	for(int i = 0; i < LSTable.size(); i++){
		std::set<Value*> preStoreAddrSet = LSaddrSets[i].second;
		for(int j = i+1; j < LSTable.size(); j++){
			//RAW
			std::set<Value*> LoadAddrSet = LSaddrSets[j].first;
			for(auto LoadAddr:LoadAddrSet){
				if(preStoreAddrSet.count(LoadAddr)){
					for(auto src:LSaddrTable[i][LoadAddr].second){
						for(auto dst:LSaddrTable[j][LoadAddr].first){
							std::string scrName = src->getName() + "_" + std::to_string(i);
							std::string dstName = dst->getName() + "_" + std::to_string(j);
							outs() << scrName << " -> " << dstName << " label = \"RAW\"\n";
							ofs << scrName << " -> " << dstName << "[color = blue, label = \"RAW\"];\n";
						}
					}
				}
			}
			//WAW
			std::set<Value*> StoreAddrSet = LSaddrSets[j].second;
			for(auto StoreAddr:StoreAddrSet){
				if(preStoreAddrSet.count(StoreAddr)){
					for(auto src:LSaddrTable[i][StoreAddr].second){
						for(auto dst:LSaddrTable[j][StoreAddr].second){
							std::string scrName = src->getName() + "_" + std::to_string(i);
							std::string dstName = dst->getName() + "_" + std::to_string(j);
							outs() << scrName << " -> " << dstName << " label = \"RAW\"\n";
							ofs << scrName << " -> " << dstName << "[color = blue, label = \"WAW\"];\n";
						}
					}
				}
			}
		}
	}
	ofs << "}\n";
	ofs.close();
}


namespace {
    cl::opt<std::string> targetFuncName("fn", cl::init("na"), cl::desc("function name"));
    cl::opt<bool> noACC("noACC", cl::init(false), cl::desc("forbid extraction of ACC-series operators"));
    cl::opt<bool> noPattern("noPattern", cl::init(false), cl::desc("forbid the extraction of memory access pattern"));
    // cl::opt<std::string> cdfgType("type", cl::init("PartPred"), cl::desc("cdfg type, valid types = PartPred, Trig, TrMap, BrMap, DFGDISE"));

    struct CDFGPass : public FunctionPass {
        static char ID;
	    CDFGPass() : FunctionPass(ID){}
        // function annotation: size attribute
        std::map<std::string, int> SizeAttrMap;
        // basic block pairs that there is a back edge in between, <srcBB, dstBB>
        SmallVector<std::pair<const BasicBlock *, const BasicBlock *>, 1> BackEdgeBBPs;    
        // the recursively successive basic blocks except the back-edge BBs    
        std::map<const BasicBlock *, std::set<const BasicBlock *>> SuccBBsMap;
        // BBs in the target loop
        std::set<const BasicBlock *> LoopBBs;
        // mapping unit to CGRA/FGRA: loop, <loop-name, MappingUnit>
        std::map<std::string, MappingUnit> MappingUnitMap;
        // BB to the corresponding loop (MappingUnit)
        // std::map<BasicBlock *, std::string> BB2MUnitMap;
		
		//a set of nested-loops
		std::map<int, Loop*> nestloops;

        virtual void getAnalysisUsage(AnalysisUsage &AU) const {	    
	    	AU.setPreservesAll();
	    	AU.addRequired<LoopInfoWrapperPass>();
	    	AU.addRequired<ScalarEvolutionWrapperPass>();
	    	// AU.addRequired<AAResultsWrapperPass>();
	    	AU.addRequired<DominatorTreeWrapperPass>();
	    	AU.addRequired<DependenceAnalysisWrapperPass>();
	    	AU.addRequiredID(LoopSimplifyID);
	    	AU.addRequiredID(LCSSAID);
	    	AU.addPreserved<DominatorTreeWrapperPass>();
	    }

	    virtual bool runOnFunction(Function &F) {
            std::string funcName = F.getName().str();
            if((targetFuncName != "na") && (targetFuncName != funcName)){        
				//errs() << funcName <<"\n\n\n\n";        
                return false;
            }
            std::string cfgName = funcName + "_cfg.dot";
            std::error_code EC;
            raw_fd_ostream File(cfgName, EC, sys::fs::OF_Text);
            if(!EC){
                WriteGraph(File, (const Function *)&F);
            }else{
                errs() << "Cannot open cfg file for WriteGraph!\n";
            }
			File.close();
            ParseSizeAttr(F, SizeAttrMap);
            LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
		    ScalarEvolution *SE = &getAnalysis<ScalarEvolutionWrapperPass>().getSE();
		    DependenceInfo *DI = &getAnalysis<DependenceAnalysisWrapperPass>().getDI();
		    DominatorTree *DT = &getAnalysis<DominatorTreeWrapperPass>().getDomTree();
		    const DataLayout &DL = F.getParent()->getDataLayout();

            // find the back edges between BBs
            FindFunctionBackedges(F, BackEdgeBBPs);
            // get SuccBBsMap
            getSuccBBsMap(F, BackEdgeBBPs, SuccBBsMap);
            printSuccBBsMap(SuccBBsMap);
            // get inner-most loops
            std::vector<Loop *> loops;
            // std::vector<Loop *> innerMostLoops;
            std::map<Loop *, std::string> loopNames;
            for(auto iter = LI.begin(); iter != LI.end(); iter++){
                loops.push_back(*iter);
            }
            getInnerMostLoops(MappingUnitMap, loopNames, loops, "LN");
            printMappingUnitMap(MappingUnitMap);
			outs() << ">>>>>> get Function input arguments <<<<<<\n";
			std::map<int, Value*> funcArgs;
			for(auto iter = F.arg_begin(); iter != F.arg_end(); iter++){
				iter->dump();
				funcArgs[iter->getArgNo()] = &*iter;
			}
			///TODO: to support multi-loops in the same level
            auto LoopVec = getMappingUnitNameUsingLoopMark(F, MappingUnitMap, SuccBBsMap);//check LoopMark first
			if(LoopVec.empty()){
				std::string munitName = getMappingUnitNameUsingPleasemapme(F, MappingUnitMap);//then check please_map_me()
				Loop *tem = MappingUnitMap[munitName].lp;
				int i = 0;
				nestloops.clear();
				while (tem != nullptr)
				{
					nestloops[i] = tem;
					i++;
					tem = tem->getParentLoop();
				}
				outs() << ">>>>>> Print the whole flattened DFG <<<<<<\n";
				Loop *outtermostLoop = nestloops[i - 1];
				MappingUnit OutterMostUnit;
				OutterMostUnit.lp = outtermostLoop;
				OutterMostUnit.allBlocks.insert(outtermostLoop->block_begin(), outtermostLoop->block_end());
				std::vector<Loop *> loopStack;
				loopStack.push_back(outtermostLoop);
				while (loopStack.size() != 0)
				{
					Loop *loop = loopStack.back();
					loopStack.pop_back();
					OutterMostUnit.entryBlocks.insert(std::make_pair(loop->getLoopPredecessor(), loop->getHeader()));
					SmallVector<std::pair<BasicBlock *, BasicBlock *>, 8> exitEdges;
					loop->getExitEdges(exitEdges);
					OutterMostUnit.exitBlocks.insert(exitEdges.begin(), exitEdges.end());
					loopStack.insert(loopStack.end(), loop->getSubLoops().begin(), loop->getSubLoops().end());
				}
				std::map<std::string, MappingUnit> temmap;
				temmap["the whole DFG"] = OutterMostUnit;
				printMappingUnitMap(temmap);
				LLVMCDFG *CDFG = new LLVMCDFG(munitName);
				CDFG->SE = SE;
				CDFG->DT = DT;
				CDFG->DI = DI;
				CDFG->DL = &DL;
				if(noACC){
					CDFG->_noACC = true;
				}
				if(noPattern){
					CDFG->_noPattern = true;
				}
				CDFG->setBackEdgeBBPs(BackEdgeBBPs);
				CDFG->setSuccBBMap(SuccBBsMap);
				CDFG->setLoopBBs(OutterMostUnit.allBlocks, OutterMostUnit.entryBlocks, OutterMostUnit.exitBlocks);
				CDFG->setLoops(nestloops);
				CDFG->setFuncArgs(funcArgs);
				// CDFG->LoopIdxAnalyze();
				CDFG->initialize();
				CDFG->printDOT("DFG_ori.dot");
				// CDFG->affineAnalyze();
				outs() << "########################################################\n";
				outs() << "Generate CDFG Started\n";
				CDFG->printHierarchyDOT("hierarchyDOT.dot");
				CDFG->generateCDFG();
			}
			else
			{
				std::map<int, std::pair<std::set<LLVMCDFGNode*>, std::set<LLVMCDFGNode*>>> LSTable;
				std::map<int, LLVMCDFG*> CDFGs;
				int kernel = 0;
				while (!LoopVec.empty())
				{	
					nestloops = LoopVec.back();//To obey the kernels' order of execution
					LoopVec.pop_back();
					outs() << ">>>>>> Print the whole flattened DFG <<<<<<\n";
					Loop *outtermostLoop = nestloops[nestloops.size()-1];
					MappingUnit OutterMostUnit;
					OutterMostUnit.lp = outtermostLoop;
					OutterMostUnit.allBlocks.insert(outtermostLoop->block_begin(), outtermostLoop->block_end());
					std::vector<Loop *> loopStack;
					loopStack.push_back(outtermostLoop);
					while (loopStack.size() != 0)
					{
						Loop *loop = loopStack.back();
						loopStack.pop_back();
						OutterMostUnit.entryBlocks.insert(std::make_pair(loop->getLoopPredecessor(), loop->getHeader()));
						SmallVector<std::pair<BasicBlock *, BasicBlock *>, 8> exitEdges;
						loop->getExitEdges(exitEdges);
						OutterMostUnit.exitBlocks.insert(exitEdges.begin(), exitEdges.end());
						///TOFIX: here include all subloops
						loopStack.insert(loopStack.end(), loop->getSubLoops().begin(), loop->getSubLoops().end());
					}
					errs() << "[][][]entryBlocks:\n";
					for(auto iter:OutterMostUnit.entryBlocks){
						errs() << iter.first->getName() <<"->>"<< iter.second->getName()<<";";
					}
					errs() << "[][][]entryBlocks\n";
					std::map<std::string, MappingUnit> temmap;
					temmap["the whole DFG"] = OutterMostUnit;
					printMappingUnitMap(temmap);
					LLVMCDFG *CDFG = new LLVMCDFG("kernel_"+std::to_string(kernel));
					CDFG->SE = SE;
					CDFG->DT = DT;
					CDFG->DI = DI;
					CDFG->DL = &DL;
					if(noACC){
						CDFG->_noACC = true;
					}
					if(noPattern){
						CDFG->_noPattern = true;
					}
					CDFG->setBackEdgeBBPs(BackEdgeBBPs);
					CDFG->setSuccBBMap(SuccBBsMap);
					CDFG->setLoopBBs(OutterMostUnit.allBlocks, OutterMostUnit.entryBlocks, OutterMostUnit.exitBlocks);
					CDFG->setLoops(nestloops);
					CDFG->setFuncArgs(funcArgs);
					// CDFG->LoopIdxAnalyze();
					CDFG->initialize();
					CDFG->printDOT("Ori_"+std::to_string(kernel)+".dot");
					// CDFG->affineAnalyze();
					CDFG->printHierarchyDOT("hierarchyDOT_"+std::to_string(kernel)+".dot");
					CDFG->generateCDFG();
					extractLSInfo(CDFG, kernel, &LSTable);
					CDFGs[kernel] = CDFG;
					kernel++;
				}
				if(CDFGs.size()>1){
					outs() << "########################################################\n";
					outs() << "Tasks Schedule Started\n";
					scheduleTasks(CDFGs, LSTable);
					outs() << "Tasks Schedule Ended\n";
					outs() << "########################################################\n";
				}
			}
            return true;

        } // end runOnFunction
    };
} // end namespace


char CDFGPass::ID = 0;
static RegisterPass<CDFGPass> X("cdfg", "CDFGPass", false, false);