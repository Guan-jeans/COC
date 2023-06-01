
#include "llvm_cdfg.h"

//#include "cgra.h"



LLVMCDFG::~LLVMCDFG()
{
    for(auto& elem : _nodes){
        delete elem.second;
    }
    for(auto& elem : _edges){
        delete elem.second;
    }
}


LLVMCDFGNode* LLVMCDFG::node(int id)
{
    if(_nodes.count(id)){
        return _nodes[id];
    }
    return NULL;
}

LLVMCDFGNode* LLVMCDFG::node(Instruction *ins)
{
    if(_insNodeMap.count(ins)){
        return _insNodeMap[ins];
    }
    return NULL;
}


void LLVMCDFG::addNode(LLVMCDFGNode *node)
{
    int id = node->id();
    if(_nodes.count(id)){
        return;
    }
    _nodes[id] = node;
    if(auto ins = node->instruction()){
        _insNodeMap[ins] = node;
    }
}

// create node according to instruction and add node
LLVMCDFGNode* LLVMCDFG::addNode(Instruction *ins)
{
    if(_insNodeMap.count(ins)){
        return _insNodeMap[ins];
    }
    // create new node
    LLVMCDFGNode *node = new LLVMCDFGNode(ins, this);
    int id = 0;
    if(!_nodes.empty()){
        id = _nodes.rbegin()->first + 1;
    }
    node->setId(id);
    node->setBB(ins->getParent());
    _nodes[id] = node;
    _insNodeMap[ins] = node;
    return node;
}

// create node according to the custom instruction and add node
LLVMCDFGNode* LLVMCDFG::addNode(std::string customIns, BasicBlock *BB)
{
    LLVMCDFGNode *node = new LLVMCDFGNode(this);
    node->setCustomInstruction(customIns);
    int id = 0;
    if(!_nodes.empty()){
        id = _nodes.rbegin()->first + 1;
    }
    node->setId(id);
    node->setBB(BB);
    _nodes[id] = node;    
    return node;
}

// create node according to the custom instruction for pattern which is not with BB: TODO ?
LLVMCDFGNode* LLVMCDFG::addNode(std::string customIns)
{
    LLVMCDFGNode *node = new LLVMCDFGNode(this);
    node->setCustomInstruction(customIns);
    int id = 0;
    if(!_nodes.empty()){
        id = _nodes.rbegin()->first + 1;
    }
    node->setId(id);
    _nodes[id] = node;    
    return node;
}


void LLVMCDFG::delNode(LLVMCDFGNode *node)
{
    for(auto inputnode:node->inputNodes()){
        //errs() << "tem inputnode is "<<inputnode->getName()<<"\n";
        inputnode->delOutputNode(node);
    }
    for(auto outputnode:node->outputNodes()){
        outputnode->delInputNode(node);
    }
    int id = node->id();
    _nodes.erase(id);
    if(auto ins = node->instruction()){
        _insNodeMap.erase(ins);
    }
    auto inputEdges = node->inputEdges();
    for(int eid : inputEdges){
        delEdge(eid);
    }
    auto outputEdges = node->outputEdges();
    for(int eid : outputEdges){
        delEdge(eid);
    }
    delete node;
}


void LLVMCDFG::delNode(Instruction *ins)
{
    if(!_insNodeMap.count(ins)){
        return;
    }
    auto node = _insNodeMap[ins];
    for(auto inputnode:node->inputNodes()){
        inputnode->delOutputNode(node);
    }
    for(auto outputnode:node->outputNodes()){
        outputnode->delInputNode(node);
    }
    _nodes.erase(node->id());
    _insNodeMap.erase(ins);
    auto inputEdges = node->inputEdges();
    for(int eid : inputEdges){
        delEdge(eid);
    }
    auto outputEdges = node->outputEdges();
    for(int eid : outputEdges){
        delEdge(eid);
    }
    delete node;
}

void LLVMCDFG::delNodeTree(LLVMCDFGNode *node){
    int id = node->id();
    if(!_nodes.count(id)) return;
    auto teminputs = node->inputNodes();
    for(auto prenode : teminputs){
        if(prenode->outputEdges().size() == 1){
            //errs()<< "\tdelete tree of "<< prenode->getName()<<"\n";
            delNodeTree(prenode);
        }
    }
    errs()<< "\tdelete node "<<node->getName()<<"\n";
    delNode(node);
}

void LLVMCDFG::delNodeTree(Instruction *ins){
    if(!_insNodeMap.count(ins)){
        return;
    }
    auto node = _insNodeMap[ins];
    auto teminputs = node->inputNodes();
    for(auto prenode : teminputs){
        if(prenode->outputEdges().size() == 1){
            delNodeTree(prenode);
        }
    }
    delNode(node);
}

void LLVMCDFG::connectNodes(LLVMCDFGNode *src, LLVMCDFGNode *dst, int idx, EdgeType edgeType, bool isBackEdge){
    src->addOutputNode(dst, isBackEdge);
    dst->addInputNode(src, idx, isBackEdge);
    addEdge(src, dst, edgeType);
}


LLVMCDFGEdge* LLVMCDFG::edge(int id)
{
    if(_edges.count(id)){
        return _edges[id];
    }
    return NULL;
}


LLVMCDFGEdge* LLVMCDFG::edge(LLVMCDFGNode *src, LLVMCDFGNode *dst)
{
    for(auto eid : src->outputEdges()){
        assert(_edges.count(eid));
        auto outEdge = _edges[eid];
        if(outEdge->dst() == dst){
            return outEdge;
        }
    }
    return NULL;
}


void LLVMCDFG::addEdge(LLVMCDFGEdge *edge)
{
    int id = edge->id();
    if(!_edges.count(id)){
        _edges[id] = edge;
        edge->src()->addOutputEdge(id);
        edge->dst()->addInputEdge(id);
    }
}


LLVMCDFGEdge* LLVMCDFG::addEdge(LLVMCDFGNode *src, LLVMCDFGNode *dst, EdgeType type)
{
    int eid = 0;
    if(!_edges.empty()){
        eid = _edges.rbegin()->first + 1;
    }
    LLVMCDFGEdge *edge = new LLVMCDFGEdge(eid, type, src, dst);
    _edges[eid] = edge;
    src->addOutputEdge(eid);
    dst->addInputEdge(eid);
    return edge;
}



void LLVMCDFG::delEdge(LLVMCDFGEdge *edge)
{
    int id = edge->id();
    if(_edges.count(id)){
        _edges.erase(id);
    }
    edge->src()->delOutputEdge(id);
    edge->dst()->delInputEdge(id);
    edge->dst()->delInputNode(edge->src());
    edge->src()->delOutputNode(edge->dst());
    delete edge;    
}


void LLVMCDFG::delEdge(int id)
{
    if(!_edges.count(id)){
       return;
    }
    auto edge = _edges[id];
    edge->src()->delOutputEdge(id);
    edge->dst()->delInputEdge(id);
    edge->dst()->delInputNode(edge->src());
    edge->src()->delOutputNode(edge->dst());
    _edges.erase(id);
    delete edge;    
}


// get GEP node info 
std::string LLVMCDFG::GEPInfo(LLVMCDFGNode* node)
{
    assert(_GEPInfoMap.count(node));
    return _GEPInfoMap[node];
}


void LLVMCDFG::setGEPInfo(LLVMCDFGNode* node, std::string name)
{
    _GEPInfoMap[node] = name;
}


// get Input/Output node info 
std::string LLVMCDFG::IOInfo(LLVMCDFGNode* node)
{
    assert(_IOInfoMap.count(node));
    return _IOInfoMap[node];
}


void LLVMCDFG::setIOInfo(LLVMCDFGNode* node, std::string name)
{
    _IOInfoMap[node] = name;
}


// initialize CDFG according to loopBBs
void LLVMCDFG::initialize()
{
    std::set<BasicBlock*> loopBBs = _loopBBs;
    for(auto &elem : _loopentryBBs){
        loopBBs.insert(elem.first);
    }
    outs() << ">>>>>>>>>>>>>initialize<<<<<<<<<<<<<\n";
    auto F = (*(loopBBs.begin()))->getParent();//to check all ConstantExpr GEP
    for(auto &BB : *F){
        //errs()<< "check BB: ";BB.dump();
        for(auto &I : BB){
            int addressidx;
            GetElementPtrInst *GEP;
            Instruction *ins = &I;
            if(auto Load = dyn_cast<LoadInst>(ins)){
                GEP = dyn_cast<GetElementPtrInst>(Load->getPointerOperand());
                addressidx = Load->getPointerOperandIndex();
            }else if(auto Store = dyn_cast<StoreInst>(ins)){
                GEP = dyn_cast<GetElementPtrInst>(Store->getPointerOperand());
                addressidx = Store->getPointerOperandIndex();
            }else{
                continue;
            }
            //errs()<< "check LS: ";ins->dump();
            if(ConstantExpr * CE = dyn_cast<ConstantExpr>(ins->getOperand(addressidx))){
                //errs()<<"LS has ConstantExpr GEP: "; ins->dump();
                std::vector<Value *> Indices;
                for (unsigned index = 1; index < CE->getNumOperands(); ++index) {
                    Indices.push_back (CE->getOperand(index));
                }
                GEP = GetElementPtrInst::CreateInBounds(nullptr,CE->getOperand(0),Indices,CE->getName(),ins);
                ins->replaceUsesOfWith(CE, GEP);
            }
        }
    }
    // Create CDFG nodes
    for(auto BB : loopBBs){
        for(auto &I : *BB){
            addNode(&I);
        }
    }
    for(auto iter = PHItoLevel.begin(); iter != PHItoLevel.end(); iter++){
        auto Ins = iter->first;
        addNode(Ins);
    }
    // create connections
    for(auto BB : loopBBs){
        for(auto &I : *BB){
            Instruction *ins = &I;
            LLVMCDFGNode *node = this->node(ins);            
            // find out-of-loop connections or constant operands for non-phi nodes
            if(!dyn_cast<PHINode>(ins)){
                int idx = 0;
                for(Use &pred : ins->operands()){
                    // add input nodes
                    LLVMCDFGNode *inputNode = NULL;
                    if(ConstantInt *CI = dyn_cast<ConstantInt>(pred)){
                        if(dyn_cast<GetElementPtrInst>(ins)){ // GEP Constant will be handled in the handleGEPNodes
                            idx++;
                            continue;
                        }
                        inputNode = addNode("CONST", node->BB());
                        inputNode->setConstVal(CI->getSExtValue());
                    }else if(Instruction *predIns = dyn_cast<Instruction>(pred)){
                        if(!loopBBs.count(predIns->getParent())){ // out of loop BB
                            if(auto LI = dyn_cast<LoadInst>(predIns)){
                                inputNode = addNodeTree(predIns);
                            }else{
                                inputNode = getInputNode(predIns, BB);
                            }
                        }
                    }else if(Argument *arg = dyn_cast<Argument>(pred)){
                        if(!pred->getType()->isPointerTy()){
                            inputNode = getInputNode(arg, BB);
                            inputNode->setCustomInstruction("ArgIn");
                        }
                    }
                    EdgeType type = EDGE_TYPE_DATA;
                    if(pred->getType()->getPrimitiveSizeInBits() == 1){ //  single-bit operand
                        type = EDGE_TYPE_CTRL;
                    }    
                    if(inputNode){
                        int opidx = idx;
                        if(dyn_cast<SelectInst>(ins) && (idx > 0)){
                            opidx--;
                        }
                        node->addInputNode(inputNode, opidx);
                        inputNode->addOutputNode(node);
                        addEdge(inputNode, node, type);
                    }
                    idx++;
                }
            }
            if(dyn_cast<PHINode>(ins))
                    ins->dump(); 
            for(User *succ : ins->users()){               
                if(Instruction *succIns = dyn_cast<Instruction>(succ)){
                    if(dyn_cast<PHINode>(ins)){ 
                        errs()<<"\t";succIns->dump();
                    }
                    LLVMCDFGNode *succNode = NULL;
                    bool isBackEdge = false;
                    if(!loopBBs.count(succIns->getParent())){ // out of loop BB
                        succNode = getOutputNode(succIns, BB);
                        //succNode->setInstruction(succIns);//record output direct ins
                    }else if(dyn_cast<PHINode>(succIns)){
                        continue; // not connected here, instead in handlePHINodes
                    }else{
                        succNode = this->node(succIns);
                        // // only if the successor is PHINode, the back edge can exist.
                        // // the following condition should never be true
                        // if(BB != succIns->getParent()){ // not in the same BB
                        //     std::pair<const BasicBlock*, const BasicBlock*> bbp(BB, succIns->getParent());
                        //     if(std::find(_backEdgeBBPs.begin(), _backEdgeBBPs.end(), bbp) != _backEdgeBBPs.end()){
                        //         isBackEdge = true;
                        //     }
                        // }
                    }
                    // get the input index
                    int idx = -1;
                    // reorder the operand of SELECT and STORE node
                    if(auto SI = dyn_cast<SelectInst>(succIns)){
                        if(ins == dyn_cast<Instruction>(SI->getCondition())){
                            idx = 2;
                        }else if(ins == dyn_cast<Instruction>(SI->getTrueValue())){
                            idx = 0;
                        }else if(ins == dyn_cast<Instruction>(SI->getFalseValue())){
                            idx = 1;
                        }else{
                            assert(false);
                        }
                    }else if(auto SI = dyn_cast<StoreInst>(succIns)){
                        if(ins == dyn_cast<Instruction>(SI->getValueOperand())){
                            idx = 0;
                        }else{
                            idx = 1;
                        }
                    }else{
                        for(int i = 0; i < succIns->getNumOperands(); i++){
                            if(succNode->getInputPort(i) == node){//when check the same port, continue to avoid break too early
                                continue;
                            }
                            if(ins == dyn_cast<Instruction>(succIns->getOperand(i))){
                                idx = i;
                                break;
                            }
                        }
                    }   
                    EdgeType type = EDGE_TYPE_DATA;
                    if(ins->getType()->getPrimitiveSizeInBits() == 1){ //  single-bit operand
                        type = EDGE_TYPE_CTRL;
                    }                 
                    node->addOutputNode(succNode, isBackEdge);
                    succNode->addInputNode(node, idx, isBackEdge);
                    addEdge(node, succNode, type);
                }
            }
        }
    }  
    /*
    //find output node & check if store
    auto temnode = _nodes;
    for(auto &elem : temnode){
        auto node = elem.second;
        if(node->customInstruction() == "OUTPUT"){
            assert(node->inputNodes().size() == 1);
            auto result = *(node->inputNodes().begin());
            Instruction* resultins = result->instruction();
            assert(resultins != NULL);
            auto outputset = addOutputTree(result->instruction());//TODO: ret output maybe deleted
            errs() << "For output: "<<node->getName()<<"\n";
            for(auto output:outputset){
                output.first->instruction()->dump();
                result->addOutputNode(output.first);
                output.first->addInputNode(result, output.second);
                EdgeType type = EDGE_TYPE_DATA;
                if(resultins->getType()->getPrimitiveSizeInBits() == 1){
                    type = EDGE_TYPE_CTRL;
                }
                addEdge(result, output.first, EDGE_TYPE_DATA);
                Instruction* outins = output.first->instruction();
                assert(outins != NULL);
            }
            errs()<< "\n";
            result->delOutputNode(node);///aviod access this node latter
            delNode(node);
        }else if(node->customInstruction() == "INPUT"){
            //auto tempoutput = node->outputNodes();
            for(auto output:node->outputNodes()){
                Instruction* ins = output->instruction();
                assert(ins != NULL);
                int idx = output->getInputIdx(node);
                auto operand = ins->getOperand(idx);
                auto newnode = addNodeTree(operand);
                if(newnode == NULL)
                    continue;
                newnode->addOutputNode(output);
                output->addInputNode(newnode, idx);
                EdgeType type = EDGE_TYPE_DATA;
                if(operand->getType()->getPrimitiveSizeInBits() == 1){
                    type = EDGE_TYPE_CTRL;
                }
                addEdge(newnode, output, type);
            }
            delNode(node);
        }
    }
    */
}



// add edge between two nodes that have memory dependence (loop-carried)
void LLVMCDFG::addMemDepEdges()
{
    std::vector<LLVMCDFGNode *> LSNodes; // load/store nodes
    // find the load/store nodes
    for(auto &elem : _nodes){
        auto node = elem.second;
        Instruction *ins = node->instruction();
        if(ins == NULL){
            continue;
        }
        if(LoadInst *LI = dyn_cast<LoadInst>(ins)){
            if(LI->isSimple()){
                LSNodes.push_back(node);
            }
        }else if(StoreInst *SI = dyn_cast<StoreInst>(ins)){
            if(SI->isSimple()){
                LSNodes.push_back(node);
            }
        }
    }
    // analyze dependence between every two LSNodes
    int N = LSNodes.size();
    for(int i = 0; i < N; i++){
        LLVMCDFGNode *srcNode = LSNodes[i];
        Instruction *srcIns = srcNode->instruction();
        for(int j = i + 1; j < N; j++){
            LLVMCDFGNode *dstNode = LSNodes[j];
            Instruction *dstIns = dstNode->instruction();
            if(auto D = DI->depends(srcIns, dstIns, true)){
                outs() << "Found memory dependence between " << LSNodes[i]->getName() 
                       << "(src) and " << LSNodes[j]->getName() << "(dst)\n";
                if(D->isLoopIndependent()){
                    outs() << "Loop independent(skipped)\n";
                    continue;
                }
                outs() << "Loop carried dependence\n";
                DepType type = NON_DEP; // dependence type
                if(D->isFlow()){ // RAW, read after write
                    type = FLOW_DEP;
                    outs() << "FLOW_DEP, ";
                }else if(D->isAnti()){ // WAR, write after read
                    type = ANTI_DEP;
                    outs() << "ANTI_DEP, ";
                }else if(D->isOutput()){ // WAW
                    type = OUTPUT_DEP;
                    outs() << "OUTPUT_DEP, ";
                }else if(D->isInput()){ // RAR
                    type = INPUT_DEP;
                    outs() << "INPUT_DEP, ";
                    continue;
                }               
                int loopIterDist; // loop-carried iteration distance, e.g. a[i+1] = a[i] : 1
                bool isLoopDistConst = 0;
                int nestedLevels = D->getLevels(); // nested loop levels, [1, nestedLevels], 1 is the outer-most loop
                assert(nestedLevels > 0);
                // target at the inner-most loop
                const SCEV *dist = D->getDistance(nestedLevels);
                const SCEVConstant *distConst = dyn_cast_or_null<SCEVConstant>(dist);
                bool reverse = false;
                if(distConst){
                    loopIterDist = distConst->getAPInt().getSExtValue();
                    outs() << "const distance: " << loopIterDist << "\n";
                    isLoopDistConst = true;
                    if(loopIterDist < 0){
                        reverse = true;
                        loopIterDist = -loopIterDist;
                        if(type == FLOW_DEP){
                            type = ANTI_DEP;
                        }else if(type == ANTI_DEP){
                            type = FLOW_DEP;
                        }
                    }                   
                }else{ /// if no subscript in the source or destination mention the induction variable associated with the loop at this level.
                    isLoopDistConst = false;   
                    outs() << "non-const distance\n";                               
                }                
                // add mem dep edges
                DependInfo dep;
                dep.type = type;
                dep.isConstDist = isLoopDistConst;
                dep.distance = loopIterDist;
                if(reverse){
                    dstNode->addDstDep(srcNode, dep);
                    srcNode->addSrcDep(dstNode, dep);
                    addEdge(dstNode, srcNode, EDGE_TYPE_MEM);
                }else{
                    srcNode->addDstDep(dstNode, dep);
                    dstNode->addSrcDep(srcNode, dep);
                    addEdge(srcNode, dstNode, EDGE_TYPE_MEM);
                }                
            }
        }
    }
}


// root nodes : no output nodes
std::vector<LLVMCDFGNode *> LLVMCDFG::getRoots()
{
	std::vector<LLVMCDFGNode *> rootNodes;
    for(auto &elem : _nodes){
        if(elem.second->outputNodes().size() == 0){
            rootNodes.push_back(elem.second);
        }
    }
	return rootNodes;
}

// leaf nodes : no input nodes
std::vector<LLVMCDFGNode *> LLVMCDFG::getLeafs()
{
	std::vector<LLVMCDFGNode *> leafNodes;
    for(auto &elem : _nodes){
        if(elem.second->inputNodes().size() == 0){
            leafNodes.push_back(elem.second);
        }
    }
	return leafNodes;
}

// leaf nodes in a BB : no input nodes in this BB or is INPUT/PHI
std::vector<LLVMCDFGNode *> LLVMCDFG::getLeafs(BasicBlock *BB)
{
	std::vector<LLVMCDFGNode *> leafNodes;
    for(auto &elem : _nodes){
        auto node = elem.second;
        if(node->BB() != BB || node->customInstruction() == "INPUT"){
            continue;
        }
        if(node->instruction() && dyn_cast<PHINode>(node->instruction())){
            continue;
        }
        bool flag = true;
        for(auto &inNode : node->inputNodes()){
            if(inNode->BB() == BB && inNode->customInstruction() != "INPUT"){
                if(inNode->instruction() == NULL || !dyn_cast<PHINode>(inNode->instruction())){
                    flag = false;
                    break;
                }
            }
        }
        if(flag){
            leafNodes.push_back(node);
        }
    }
	return leafNodes;
}


// return a map of basicblocks to their real control dependent (recursive) predecessors with the respective control value
std::map<BasicBlock*, std::set<std::pair<BasicBlock*, CondVal>>> LLVMCDFG::getCtrlDepPredBBs() {
    //to record exitingblock for each level
    std::map<BasicBlock*, int> exitingBlocks;
    for(auto iter : nestloops()){
       BasicBlock* exitingBB = iter.second->getExitingBlock();
       assert(exitingBB != NULL);
       exitingBlocks[exitingBB] = iter.first;
    }
	std::map<BasicBlock*,std::set<std::pair<BasicBlock*,CondVal>>> res;
    // BFS to find all the recursive predecessors except the out-of-loop and back-edge ones for each BB
	for(BasicBlock* BB : _loopBBs){
        int temlevel;
        for(auto iter : _LoopexclusiveBBs){
            if(iter.second.count(BB)){
                temlevel = iter.first;
                break;
            }
        }
		std::queue<BasicBlock*> q;
		q.push(BB);
		std::map<BasicBlock*,std::set<CondVal>> visited;
		while(!q.empty()){
			BasicBlock* curr = q.front(); q.pop();
			for (auto it = pred_begin(curr); it != pred_end(curr); ++it){
				BasicBlock* predecessor = *it;
				if(_loopBBs.find(predecessor) == _loopBBs.end()){
					continue; // no need to care for out of loop BBs.
				}
				std::pair<const BasicBlock*,const BasicBlock*> bbPair = std::make_pair(predecessor, curr);
				if(std::find(_backEdgeBBPs.begin(), _backEdgeBBPs.end(), bbPair) != _backEdgeBBPs.end()){
					continue; // no need to traverse backedges;
				}
				CondVal cv;
				assert(predecessor->getTerminator());
				BranchInst* BRI = cast<BranchInst>(predecessor->getTerminator());
                // get control value
				if(!BRI->isConditional() || exitingBlocks.count(predecessor)){
					cv = UNCOND;
				}else{
					for (int i = 0; i < BRI->getNumSuccessors(); ++i) {
						if(BRI->getSuccessor(i) == curr){
							if(i==0){
								cv = TRUE_COND;
							}else if(i==1){
								cv = FALSE_COND;
							}else{
								assert(false);
							}
						}
					}
				}
				visited[predecessor].insert(cv);
				q.push(predecessor);
			}
		}

		outs() << "BasicBlock : " << BB->getName() << " :: CtrlDependentBBs = ";
		for(std::pair<BasicBlock*,std::set<CondVal>> pair : visited){
			BasicBlock* bb = pair.first;
			std::set<CondVal> brOuts = pair.second;
			outs() << bb->getName();
			if(brOuts.count(TRUE_COND)){
				res[BB].insert(std::make_pair(bb, TRUE_COND)); // TRUE control dependent predecessor BB
				outs() << "(TRUE),";
			}
			if(brOuts.count(FALSE_COND)){
				res[BB].insert(std::make_pair(bb, FALSE_COND)); // FALSE control dependent predecessor BB
				outs() << "(FALSE),";
			}
		}
		outs() << "\n";
	} // res[BB] = set<preBB, TRUE/FALSE_COND>

    // remove extra <preBB, TRUE/FALSE_COND>
	std::map<BasicBlock*, std::set<std::pair<BasicBlock*, CondVal>>> refinedRes;
	for(auto &pair : res){
		std::set<std::pair<BasicBlock*,CondVal>> tobeRemoved;
		BasicBlock* currBB = pair.first;
		outs() << "BasicBlock : " << currBB->getName() << " :: RefinedCtrlDependentBBs = ";
		for(auto &bbVal : pair.second){
			BasicBlock* depBB = bbVal.first;
			for(auto &p2 : res[depBB]){
				tobeRemoved.insert(p2);
			}
		}
        // if currBB and depBB have the same <preBB, TRUE/FALSE_COND>, remove from res[BB]
		for(auto &bbVal : pair.second){
			if(tobeRemoved.find(bbVal) == tobeRemoved.end()){
				outs() << bbVal.first->getName();
				outs() << ((bbVal.second == TRUE_COND)? "(TRUE)," : "(FALSE),");
				refinedRes[currBB].insert(bbVal);
			}
		}
		outs() << "\n";
	}

    // remove preBB with both TRUE_COND and FALSE_COND from refinedRes
	std::map<BasicBlock*,std::set<std::pair<BasicBlock*,CondVal>>> finalRes;
    for(auto &pair : refinedRes){
		BasicBlock* currBB = pair.first;
		outs() << "BasicBlock : " << currBB->getName() << " :: FinalCtrlDependentBBs = ";
		std::set<std::pair<BasicBlock*, CondVal>> bbValPairs = pair.second; // auto-sorted, first prority : BasicBlock*, second : CondVal
        assert(bbValPairs.size() > 0);
        bool changed = true;
        while(changed){
            changed = false;
            for(auto it = bbValPairs.begin(), ie = --bbValPairs.end(); it != ie;){
                auto bb1 = it->first;
                auto old_it = it;
                auto next_it = ++it;
                if(bb1 == next_it->first){
                    bbValPairs.erase(old_it, ++next_it); // remove TRUE_COND and FALSE_COND preBBs
                    if(finalRes.count(bb1)){ // already got the final control dependent BBs
                        for(auto bbp : finalRes[bb1]){
                            bbValPairs.insert(bbp);
                        }
                    }else if(refinedRes.count(bb1)){ // add control dependent BBs from refinedRes[bb1]
                        for(auto bbp : refinedRes[bb1]){
                            bbValPairs.insert(bbp);
                        }
                    }
                    changed = (bbValPairs.size() > 0); // if still have element, continue
                    break;
                }
            }
        }
        if(bbValPairs.size() > 0){
            finalRes[currBB] = bbValPairs;
            for(auto &bbVal: bbValPairs){
			    outs() << bbVal.first->getName();
			    outs() << ((bbVal.second == TRUE_COND)? "(TRUE)," : "(FALSE),");
		    }
        }
        outs() << "\n";
    }
	return finalRes;
}


// get the control dependent nodes in a BB, including unconditional BranchInst, StoreInst, OUTPUT
// only TRUE_COND, STORE can be performed
std::vector<LLVMCDFGNode*> LLVMCDFG::getCtrlDepNodes(BasicBlock *BB)
{
    std::vector<LLVMCDFGNode*> res;
    for(auto &elem : _nodes){
        auto node = elem.second;
        if(node->BB() != BB){
            continue;
        }
        if(Instruction *ins = node->instruction()){
            if(BranchInst *BRI = dyn_cast<BranchInst>(ins)){
                if(!BRI->isConditional()){
                    res.push_back(node);
                }
            }else if(dyn_cast<StoreInst>(ins)){
                res.push_back(node);
            }else if(dyn_cast<LoadInst>(ins)){
                res.push_back(node);
            }
        }
        ///TODO: maybe consider regOut here
        // else if(node->customInstruction() == "OUTPUT"){
        //     res.push_back(node);
        // }
    }
    return res;
}

// add CTRLAND each CTRL path's node to tem preBB
std::set<std::pair<LLVMCDFGNode *, CondVal>> LLVMCDFG::ANDctrlPathNodes(std::map<BasicBlock*, std::set<std::pair<BasicBlock*, CondVal>>> &CtrlDepPredBBs, BasicBlock* BB){
    
    if(_BBctrlDependentNode.count(BB)){
        return _BBctrlDependentNode[BB];
    }

    std::set<std::pair<LLVMCDFGNode *, CondVal>> result;
    result.clear();
    for(auto &ctrlDepPredBB : CtrlDepPredBBs[BB]){
        BasicBlock* preBB = ctrlDepPredBB.first;
        CondVal cond = ctrlDepPredBB.second;
		BranchInst* BRI = cast<BranchInst>(preBB->getTerminator());
        LLVMCDFGNode* BRNode = node(BRI);
        LLVMCDFGNode* preBRNode = BRNode->inputNodes()[0];
        std::set<std::pair<LLVMCDFGNode *, CondVal>> ANDNodes = ANDctrlPathNodes(CtrlDepPredBBs, preBB);
        if(ANDNodes.size() > 0){
            std::set<std::pair<LLVMCDFGNode *, CondVal>> finalANDNodes;
            for(auto &elem:ANDNodes){
                BasicBlock* sourceBB = elem.first->BB();
                CondVal sourceCond = elem.second;
                std::pair<llvm::BasicBlock *, CondVal> compPair;
                if(sourceCond == TRUE_COND){
                    compPair = std::make_pair(sourceBB, FALSE_COND);
                }else if(sourceCond == FALSE_COND){
                    compPair = std::make_pair(sourceBB, TRUE_COND);
                }else{
                    assert(false&&"why uncond?");
                }
                if(!CtrlDepPredBBs[BB].count(compPair)){
                    finalANDNodes.insert(elem);
                }
            }
            if(finalANDNodes.size() > 0){
                LLVMCDFGNode* CTRLAND = addNode("CTRLAND", BB);
                for(auto preBBCtrl : finalANDNodes){
                    CTRLAND->addInputNode(preBBCtrl.first, -1, false, preBBCtrl.second);
                    preBBCtrl.first->addOutputNode(CTRLAND, false, preBBCtrl.second);
                    addEdge(preBBCtrl.first, CTRLAND, EDGE_TYPE_CTRL);
                }
                CTRLAND->addInputNode(preBRNode, 0, false, cond);
                preBRNode->addOutputNode(CTRLAND, false, cond);
                addEdge(preBRNode, CTRLAND, EDGE_TYPE_CTRL);
                preBRNode = CTRLAND;
                cond = TRUE_COND;
            }
        }
        _BBctrlDependentNode[BB].insert(std::make_pair(preBRNode, cond));
        result = _BBctrlDependentNode[BB];
    }
    return result;
}


// Connect control dependent node pairs among BBs
void LLVMCDFG::connectCtrlDepBBs(){
    // get real control dependent (recursive) predecessors with the respective control value
	std::map<BasicBlock*, std::set<std::pair<BasicBlock*, CondVal>>> CtrlDepPredBBs = getCtrlDepPredBBs();
	for(auto &BBelem : CtrlDepPredBBs){
		BasicBlock* currBB = BBelem.first;
		std::vector<LLVMCDFGNode*> ctrlDepNodes = getCtrlDepNodes(currBB);
        //add CTRLAND each CTRL path's node to tem preBB & update _BBctrlDependentNode
        auto currBBctrlNodes = ANDctrlPathNodes(CtrlDepPredBBs, currBB);
        for(auto &elem:currBBctrlNodes){
            LLVMCDFGNode* preBRNode = elem.first;
            Instruction* preBRCMP = preBRNode->instruction();
            CondVal cond = elem.second;
            BasicBlock* preBB = preBRNode->BB();
            outs() << "ConnectBB :: From " << preBRNode->getName() << "(srcBB = " << preBB->getName() << ")" << ", To ";
			for(LLVMCDFGNode* succNode : ctrlDepNodes){
                ///DO NOT connect LoopIdx Safety control
                bool isSafetyCheck = false;
                if(preBRCMP != NULL){
                    for (auto iter : _safetyMap){
                        auto safetypair = iter.second;
                        if (safetypair.first == preBRCMP && safetypair.second == cond){
                            isSafetyCheck = true;
                            break;
                        }
                    }
                }
                if(isSafetyCheck){
                    continue;
                }
                int  idx = -1;
				outs() << succNode->getName() << ", ";
                //set condition port of CSTORE and CLOAD
                if(succNode->instruction() != NULL){
                    if(dyn_cast<StoreInst>(succNode->instruction())){
                        idx = 2;
                    }
                    if(dyn_cast<LoadInst>(succNode->instruction())){
                        idx = 1;
                    }
                }
                preBRNode->addOutputNode(succNode, false, cond);
                succNode->addInputNode(preBRNode, idx, false, cond); // donot care the index
                addEdge(preBRNode, succNode, EDGE_TYPE_CTRL);
			}
            outs() << "\n";
        }
	}
    //add complete condition of built-in select node
    for(auto &elem : _nodes){
        LLVMCDFGNode* node = elem.second;
        Instruction* ins = node->instruction();
        if(ins != NULL && dyn_cast<SelectInst>(ins)){
            node->setCustomInstruction("SELECT");
            LLVMCDFGNode* directCTRLNode = node->getInputPort(2);
            BasicBlock* selectBB = node->BB();
            auto currBBctrlNodes = _BBctrlDependentNode[selectBB];
            if(!currBBctrlNodes.empty()){
                LLVMCDFGNode* CTRLAND = addNode("CTRLAND", selectBB);
                delEdge(edge(directCTRLNode, node));
                node->addInputNode(CTRLAND, 2, false);
                CTRLAND->addOutputNode(node);
                addEdge(CTRLAND, node, EDGE_TYPE_CTRL);
                CTRLAND->addInputNode(directCTRLNode, 0, false, TRUE_COND);
                directCTRLNode->addOutputNode(CTRLAND);
                addEdge(directCTRLNode, CTRLAND, EDGE_TYPE_CTRL);
                for(auto toAnd : currBBctrlNodes){
                    LLVMCDFGNode* toAndNode = toAnd.first;
                    CondVal toAndCond = toAnd.second;
                    CTRLAND->addInputNode(toAndNode, -1, false, toAndCond);
                    toAndNode->addOutputNode(CTRLAND);
                    addEdge(toAndNode, CTRLAND, EDGE_TYPE_CTRL);
                }
            }
        }
    }
}


// insert Control NOT node behind the condition node of the Branch node
void LLVMCDFG::insertCtrlNotNodes()
{
    outs() << "insertCtrlNotNodes STARTED!\n";
    // for(auto &elem : _nodes){
    //     LLVMCDFGNode* node = elem.second;
    //     std::set<LLVMCDFGNode*> falseOutNodes;
    //     for(auto outNode : node->outputNodes()){
    //         if(node->getOutputCondVal(outNode) == FALSE_COND){
    //             falseOutNodes.insert(outNode);
    //         }
    //     }
        // if(falseOutNodes.empty()){
        //     continue;
        // }
        // // create CTRLNOT node
        // LLVMCDFGNode* notNode = addNode("CTRLNOT", node->BB());
		// outs() << "newNOTNode = " << notNode->getName() << "\n";
        // for(auto outNode : falseOutNodes){
        //     // add new connections
        //     bool isBackEdge = node->isOutputBackEdge(outNode);
        //     notNode->addOutputNode(outNode, isBackEdge, TRUE_COND);
        //     outNode->addInputNode(notNode, isBackEdge, TRUE_COND);
        //     addEdge(notNode, outNode, EDGE_TYPE_CTRL);
        //     // delete old connections
        //     node->delOutputNode(outNode);
        //     outNode->delInputNode(node);
        // }
    for(auto BB : _loopBBs){       
        BranchInst* BRI = cast<BranchInst>(BB->getTerminator());
        if(!BRI->isConditional()){
            continue;
        }
        Instruction *condIns = dyn_cast<Instruction>(BRI->getCondition()); // conditional predecessor
        LLVMCDFGNode *node = this->node(condIns);
        outs() << "insert CTRLNOT node behind " << node->getName() << ", ";
        // create CTRLNOT node
        LLVMCDFGNode* notNode = addNode("CTRLNOT", node->BB());
		outs() << "newNOTNode = " << notNode->getName() << "\n";  
        std::set<LLVMCDFGNode*> falseOutNodes;                
        for(auto outNode : node->outputNodes()){
            if(node->getOutputCondVal(outNode) == FALSE_COND){
                falseOutNodes.insert(outNode);
            }
        }
        for(auto outNode : falseOutNodes){
            bool isBackEdge = node->isOutputBackEdge(outNode); //  should be false
            // delete old connections
            node->delOutputNode(outNode);
            int idx = outNode->delInputNode(node);
            // add new connections           
            notNode->addOutputNode(outNode, isBackEdge, TRUE_COND);
            outNode->addInputNode(notNode, idx, isBackEdge, TRUE_COND);
            addEdge(notNode, outNode, EDGE_TYPE_CTRL);            
        }
        // delete old edges
        auto temp = node->outputEdges();
        for(auto eid : temp){
            auto outEdge = edge(eid);
             outs() << "outEdge NULL?(1 is empty)" << (outEdge == NULL) << "\n";
             outs() << "outEdge source: " << outEdge->src()->getName() << "\n"; //
            auto dstNode = outEdge->dst();
            if(falseOutNodes.count(dstNode)){
                delEdge(outEdge);
            }
        }
        node->addOutputNode(notNode, false, FALSE_COND);
        notNode->addInputNode(node, 0, false, FALSE_COND);
        addEdge(node, notNode, EDGE_TYPE_CTRL);
    }
///replace set of CTRLNodes by CTRLNOT
    for(auto &BB2preCTRLNODE : _BBctrlDependentNode){
        auto currBB = BB2preCTRLNODE.first;
        errs() << "current BB is: " << currBB->getName() << "\n";
        auto preCTRLs = BB2preCTRLNODE.second;
        for(auto &elem : preCTRLs){
            auto preCTRL = elem.first;
            CondVal cond = elem.second;
            errs() << "\ttem preCTRL is: "; 
            if(preCTRL->instruction()!=NULL)
                preCTRL->instruction()->dump();
            else{
                errs() << preCTRL->getName() << "\n";
            }
            if(cond == TRUE_COND){
                continue;
            }else if(cond == FALSE_COND){
                _BBctrlDependentNode[currBB].erase(std::make_pair(preCTRL, FALSE_COND));
                for(auto outputnode : preCTRL->outputNodes()){
                    if(outputnode->customInstruction() == "CTRLNOT"){
                        _BBctrlDependentNode[currBB].insert(std::make_pair(outputnode, TRUE_COND));
                        break;
                    }
                }
            }else{
                assert(0 && "what's wrong CTRLNOT");
            }
        }
    }
    outs() << "insertCtrlNotNodes ENDED!\n";
}


// transfer the multiple control predecessors (input nodes) into a inverted OR tree 
// with the root connected to a node and leaves connected to control predecessors
void LLVMCDFG::createCtrlOrTree() 
{
	outs() << "createCtrlOrTree STARTED!\n";
    std::map<LLVMCDFGNode*, std::set<LLVMCDFGNode*>> condParentsMap; // conditional parents Map
	for(auto &elem : _nodes){
        LLVMCDFGNode* node = elem.second;		
		for(LLVMCDFGNode* par : node->inputNodes()){
            auto cond = node->getInputCondVal(par);
            if(node->customInstruction() == "CTRLAND"){
                if(node->getInputIdx(par) == -1){
                    condParentsMap[node].insert(par);
                }
            }else{
                if( cond != UNCOND){
                    condParentsMap[node].insert(par);
                    // condParents.insert(par);
                }
            }
		}
    }
    for(auto &elem : condParentsMap){
        LLVMCDFGNode* node = elem.first;
        auto &condParents = elem.second; // conditional parents
        // create CTRLOR tree
		if(condParents.size() > 1){
			std::queue<LLVMCDFGNode*> q;
			for(auto pp : condParents){
				q.push(pp);
			}
			while(!q.empty()){
				auto pp1 = q.front(); q.pop();
				if(!q.empty()){
					auto pp2 = q.front(); q.pop();
					outs() << "Connecting pp1 = " << pp1->getName() << ", ";
					outs() << "pp2 = " << pp2->getName() << ", ";                   
                    // add CTRLOR node
                    LLVMCDFGNode* orNode = addNode("CTRLOR", node->BB());
					outs() << "newORNode = " << orNode->getName() << "\n";                    
					bool isPP1BackEdge = node->isInputBackEdge(pp1);
					bool isPP2BackEdge = node->isInputBackEdge(pp2);
                    assert(node->getInputCondVal(pp1) == TRUE_COND); // the FALSE_COND should be transformed to TRUE_COND before calling this function
                    assert(node->getInputCondVal(pp2) == TRUE_COND);
                    // delete old connections
                    pp1->delOutputNode(node);
                    node->delInputNode(pp1);
                    pp2->delOutputNode(node);
                    node->delInputNode(pp2);     
                    delEdge(edge(pp1, node));     
                    delEdge(edge(pp2, node));  
                    // add new connections
                    pp1->addOutputNode(orNode, isPP1BackEdge, TRUE_COND);
                    orNode->addInputNode(pp1, 0, isPP1BackEdge, TRUE_COND);
                    pp2->addOutputNode(orNode, isPP2BackEdge, TRUE_COND);
                    orNode->addInputNode(pp2, 1, isPP2BackEdge, TRUE_COND);
                    orNode->addOutputNode(node, false, TRUE_COND);
                    node->addInputNode(orNode, -1, false, TRUE_COND);
                    addEdge(pp1, orNode, EDGE_TYPE_CTRL);
                    addEdge(pp2, orNode, EDGE_TYPE_CTRL);
                    addEdge(orNode, node, EDGE_TYPE_CTRL);   
                            
					q.push(orNode);
				}else{
					outs() << "Alone node = " << pp1->getName() << "\n";
				}
			}
		}
	}
	outs() << "createCtrlOrTree ENDED!\n";
    for(auto &elem : _nodes){
        auto node = elem.second;
        if(node->customInstruction() == "CTRLOR"){
            BasicBlock* currBB = node->BB();
            ///TODO: how about "Alone node"
            _BBctrlDependentNode[currBB].clear();
            _BBctrlDependentNode[currBB].insert(std::make_pair(node, TRUE_COND));
        }
        if(node->customInstruction() == "CTRLAND"){
            auto unsetOp = node->getInputPort(-1);
            node->setInputIdx(unsetOp, 1);
        }
    }
}


// get loop start node. If not exist, create one.
LLVMCDFGNode* LLVMCDFG::getLoopStartNode(BasicBlock *BB)
{
    if(_loopStartNodeMap.count(BB)){
        return _loopStartNodeMap[BB];
    }
    // create new node and add node
    LLVMCDFGNode *node = addNode("LOOPSTART", BB);
    // int cnt = _loopStartNodeMap.size();
    _loopStartNodeMap[BB] = node;
    // node->setConstVal(cnt);
    return node;
}


// get loop exit node. If not exist, create one.
LLVMCDFGNode* LLVMCDFG::getLoopExitNode(BasicBlock *BB)
{
    if(_loopExitNodeMap.count(BB)){
        return _loopExitNodeMap[BB];
    }
    // create new node and add node
    LLVMCDFGNode *node = addNode("LOOPEXIT", BB);
    // int cnt = _loopExitNodeMap.size();
    _loopExitNodeMap[BB] = node;
    // node->setConstVal(cnt);
    return node;
}

// get input node. If not exist, create one.
LLVMCDFGNode* LLVMCDFG::getInputNode(Value *ins, BasicBlock *BB)
{
    if(_ioNodeMap.count(ins)){
        return _ioNodeMap[ins];
    }
    // create new node and add node
    LLVMCDFGNode *node = addNode("INPUT", BB);
    _ioNodeMap[ins] = node;
    // _ioNodeMapReverse[node] = ins;
    setIOInfo(node, ins->getName().str());
    return node;
}


// get out loop store node. If not exist, create one.
LLVMCDFGNode* LLVMCDFG::getOutputNode(Value *ins, BasicBlock *BB)
{
    if(_ioNodeMap.count(ins)){
        return _ioNodeMap[ins];
    }
    // create new node and add node
    LLVMCDFGNode *node = addNode("OUTPUT", BB);
    _ioNodeMap[ins] = node;
    // _ioNodeMapReverse[node] = ins;
    setIOInfo(node, ins->getName().str());
    return node;
}


/*
 * This function
 * 1) connects PHI nodes with its parents by analyzing PHI instruction[value,basic block][value, basic block]
 * 2) transfer PHI nodes to SELECT nodes
 * 3) add LOOPSTART control node, INPUT data node
 *
 * */
void LLVMCDFG::handlePHINodes() 
{
	std::vector<LLVMCDFGNode*> phiNodes;
    auto temnodes = _nodes;
	for(auto &elem : temnodes){
        auto &node = elem.second;
        bool isSafetyCheck = false;
		if(node->instruction() && dyn_cast<PHINode>(node->instruction()) && node->customInstruction() != "SELECT"){
            PHINode *PHI = dyn_cast<PHINode>(node->instruction());
            for (int i = 0; i < PHI->getNumIncomingValues(); ++i)
            {
                LLVMCDFGNode *previousCtrlNode;
                BasicBlock *bb = PHI->getIncomingBlock(i);
                Value *V = PHI->getIncomingValue(i);
                BranchInst *BRI = cast<BranchInst>(bb->getTerminator());
                LLVMCDFGNode *BRNode = this->node(BRI);
                ///TODO: is this right?
                if (BRNode == NULL){
                    continue;
                }
                if (BRI->isConditional()){   
                    assert(BRNode->inputNodes().size() == 1); // only one condition predecessor
                    LLVMCDFGNode *ctrlNode = BRNode->inputNodes()[0];
                    Instruction *ins = ctrlNode->instruction();
                    CmpInst *cmp = NULL;
                    if(auto tem = dyn_cast<CmpInst>(ins)){
                        cmp = tem;
                    }else{//may be CTRLOR, continue
                        continue;
                    }
                    assert(cmp != NULL);
                    CondVal innerCond;
                    if (BRI->getSuccessor(0) == node->BB())
                    { // true condition
                        innerCond = FALSE_COND;
                    }
                    else
                    {
                        innerCond = TRUE_COND;
                    }
                    ///DO NOT connect LoopIdx Safety control
                    for (auto iter : _safetyMap)
                    {
                        auto elem = iter.second;
                        if (elem.first == ins && elem.second == innerCond)
                        {
                            errs() << "PHI :: ";
                            PHI->dump();
                            errs() << "this phi is controled by idx safety check, delete\n";
                            if (Instruction *preins = dyn_cast<Instruction>(PHI->getIncomingValue(1-i)))//TODO:: is that precise?
                            {
                                LLVMCDFGNode *prenode = LLVMCDFG::node(preins);
                                assert(prenode != NULL);
                                for (auto phioutput : node->outputNodes())
                                {
                                    EdgeType type = edge(node, phioutput)->type();
                                    int idx = phioutput->delInputNode(node);
                                    phioutput->addInputNode(prenode, idx);
                                    prenode->addOutputNode(phioutput);
                                    addEdge(prenode, phioutput, type);
                                }
                                isSafetyCheck = true;
                                delNode(node);
                            }
                            else
                            {
                                assert(0 && "what's going on about phi for idxbound safety check");
                            }
                            break;
                        }
                    }
                }
            }
            if(!isSafetyCheck){
                phiNodes.push_back(node);
            }
		}
	}
/*
    for(auto &elem : temnodes){
        auto &node = elem.second;
        if(node->instruction() && dyn_cast<PHINode>(node->instruction()) && node->customInstruction() != "SELECT"){
            phiNodes.push_back(node);
        }
    }
*/
    // PHI Node to SELECT node map
    std::map<LLVMCDFGNode*, LLVMCDFGNode*> phi2selectMap;
    // Phi node and its PHI parent before got the above map
    std::map<LLVMCDFGNode*, LLVMCDFGNode*> phi2phiParentMap;
    
	for(LLVMCDFGNode* node : phiNodes){
		PHINode* PHI = dyn_cast<PHINode>(node->instruction());
        outs() << "PHI :: "; PHI->dump();
        //TODO:is this right to skip PHI? how to generalize this
        if((!_loopBBs.count(PHI->getParent())) && (PHItoLevel.count(PHI))){
            outs() << "Outter index PHI, generate SELECT;\n";
            int idxlevel = PHItoLevel[PHI];
            BasicBlock* BB = PHI->getIncomingBlock(0);///TODO::any exception?
            auto outputs = node->outputNodes();
            std::map<LLVMCDFGNode*, int> nodestoInputidx;
            for(auto itr = outputs.begin(); itr != outputs.end(); itr++){
                nodestoInputidx[*itr] = (*itr)->getInputIdx(node);
            }
            delNode(PHI);
            auto selNode = addIdxCycle(PHI);//selNode means SELECT here
            LLVMCDFGNode* temnode;
            for(auto itr = outputs.begin(); itr != outputs.end(); itr++){
                int i = 0;
                temnode = *itr;
                int inputidx = nodestoInputidx[temnode];
                if(temnode->getInputPort(inputidx) == selNode){
                    continue;
                }
                temnode->addInputNode(selNode, inputidx);
                selNode->addOutputNode(temnode);
                addEdge(selNode, temnode, EDGE_TYPE_DATA);
            }
            //create idx level start
            LLVMCDFGNode *startnode = addNode("LOOPSTART", BB);
            selNode->addInputNode(startnode, 2);
            startnode->addOutputNode(selNode);
            addEdge(startnode, selNode, EDGE_TYPE_CTRL);

            outs() << "new SELECT node = " << selNode->getName() << "\n";
            outs() << selNode->getName() << " has inputs: "<<selNode->inputNodes().size()<<"\n";
            
            continue;
        }
		assert(node->inputNodes().empty());
        std::vector<std::pair<LLVMCDFGNode*, LLVMCDFGNode*>> phiParents; // <value-node, control-node>>
        bool hasLoopstart = false;
        int loopstartIdx;
		for (int i = 0; i < PHI->getNumIncomingValues(); ++i) {
			BasicBlock* bb = PHI->getIncomingBlock(i);
            bool outterIncoming = true;
			Value* V = PHI->getIncomingValue(i);
			outs() << "IncomingValue :: "; V->dump();
			LLVMCDFGNode* previousCtrlNode = NULL;
            for(int level = 0; level < nestloops().size(); level++){
                if(_LoopexclusiveBBs[level].count(bb)){
                    if(_LoopexclusiveBBs[level].count(node->BB())){
                        outterIncoming = false;
                    }
                    break;
                }
            }
			if(outterIncoming){ // predecessor not in loopBBs		
                errs() << node->getName() << " has outter predecessor\n";			
				std::pair<BasicBlock*,BasicBlock*> bbPair = std::make_pair(bb, node->BB());
				assert(_loopentryBBs.count(bbPair) || _loopexitBBs.count(bbPair)); // should be loop entry
                LLVMCDFGNode *startNode = getLoopStartNode(bb);
				previousCtrlNode = startNode;
                hasLoopstart = true;
                loopstartIdx = i;
			}else{ // within the loopBBs
				BranchInst* BRI = cast<BranchInst>(bb->getTerminator());
				LLVMCDFGNode* BRNode = this->node(BRI);
				if(!BRI->isConditional()){
                    auto &inNodes = BRNode->inputNodes();
                    assert(inNodes.size() == 1); // only one conditional predecessor after createCTRLORTree
                    previousCtrlNode = inNodes[0];
				}else{
					assert(BRNode->inputNodes().size() == 1); // only one condition predecessor
                    LLVMCDFGNode *ctrlNode = BRNode->inputNodes()[0];
                    if(BRI->getSuccessor(0) == node->BB()){ // true condition
                        previousCtrlNode = ctrlNode;
                    }else{ // false condition
                        assert(BRI->getSuccessor(1) == node->BB());
                        // find the CTRLNOT node
                        for(auto outNode : ctrlNode->outputNodes()){
                            if(outNode->customInstruction() == "CTRLNOT"){
                                previousCtrlNode = outNode;
                                break;
                            }
                        }
                    }
				}
			}
			assert(previousCtrlNode != NULL);
            outs() << "previousCTRLNode : " << previousCtrlNode->getName() << "\n";
            // get operand value
            LLVMCDFGNode* phiParent = NULL;
            
            if(ConstantInt* CI = dyn_cast<ConstantInt>(V)){
				int constant = CI->getSExtValue();
                phiParent = addNode("CONST", bb);
                phiParent->setConstVal(constant);
			}else if(ConstantFP* FP = dyn_cast<ConstantFP>(V)){
				int constant = (int)FP->getValueAPF().convertToFloat();
                phiParent = addNode("CONST", bb);
                phiParent->setConstVal(constant);
			}else if(UndefValue *UND = dyn_cast<UndefValue>(V)){
                phiParent = addNode("CONST", bb);
                phiParent->setConstVal(0);
			}else if(Argument *ARG = dyn_cast<Argument>(V)){
                phiParent = addNode("ArgIn", bb);
                //phiParent->setConstVal(0);
			}else{								
                if(Instruction* ins = dyn_cast<Instruction>(V)){
                    phiParent = this->node(ins);
                    if(dyn_cast<PHINode>(ins)){
                        if(!_loopBBs.count(ins->getParent()) && ins->getNumOperands() == 1){//to handle initial from outter lcssa phi
                            V = ins->getOperand(0);
                            if (ConstantInt *CI = dyn_cast<ConstantInt>(V))
                            {
                                int constant = CI->getSExtValue();
                                phiParent = addNode("CONST", bb);
                                phiParent->setConstVal(constant);
                            }
                            else if (ConstantFP *FP = dyn_cast<ConstantFP>(V))
                            {
                                int constant = (int)FP->getValueAPF().convertToFloat();
                                phiParent = addNode("CONST", bb);
                                phiParent->setConstVal(constant);
                            }
                        }
                        else if(phi2selectMap.count(phiParent)){ // PHI parent has been transformed to SELECT node
                            phiParent = phi2selectMap[phiParent];
                        }else{ // record unresolved PHI parent
                            phi2phiParentMap[node] = phiParent;
                        }                       
                    }				                    
				}
				if(phiParent == NULL){ //not found
                    phiParent = getInputNode(V, bb);
				}
            }
            phiParents.push_back(std::make_pair(phiParent, previousCtrlNode));
        }
        for(int i = 0; i + 1 < phiParents.size(); i += 2){
            // connect two parents to a SELECT node
            // operand 0 : true data; 1 : false data; 2 : condition (conditional node of parent 1)
            auto &pp1 = phiParents[i];
            auto &pp2 = phiParents[i+1];
            // create a SELECT node
            LLVMCDFGNode *selNode = addNode("SELECT", node->BB());
            //if(!PHItoLevel.count(PHI))
                selNode->setInstruction(PHI);///set Instruction if this SELECT is not index
            outs() << "new SELECT node = " << selNode->getName() << "\n";
            errs()<<"\t"<<selNode->getName()<< " in "<< node->BB()->getName() <<"\n";
            errs()<<"\tparent1: "<<pp1.first->getName()<< " in "<< pp1.first->BB()->getName() <<"\n";
            errs()<<"\tparent2: "<<pp2.first->getName()<< " in "<< pp2.first->BB()->getName() <<"\n";
            bool isBackEdge1 = false;
            //backedge is depend on incomingBB, but not which BB having the incomingValue 
            std::pair<const BasicBlock*, const BasicBlock*> bbp1 = std::make_pair(PHI->getIncomingBlock(i), node->BB());
            if(std::find(_backEdgeBBPs.begin(), _backEdgeBBPs.end(), bbp1) != _backEdgeBBPs.end()){
                isBackEdge1 = true;
                errs()<<"\tparent1 to Sel is backedge!!!\n";
            }
            bool isBackEdge2 = false;
            //backedge is depend on incomingBB, but not which BB having the incomingValue 
            std::pair<const BasicBlock*, const BasicBlock*> bbp2 = std::make_pair(PHI->getIncomingBlock(i+1), node->BB());
            if(std::find(_backEdgeBBPs.begin(), _backEdgeBBPs.end(), bbp2) != _backEdgeBBPs.end()){
                isBackEdge2 = true;
                errs()<<"\tparent2 to Sel is backedge!!!\n";
            }
            // swap two operands
            // 1. the second operand is loop initial value
            // 2. the second control node have more successive BBs
            // 3. the first control node is CTRLNOT  
            bool swapOperand = hasLoopstart && loopstartIdx == 1; 
            if(!swapOperand){
                int succBBNum1 = _succBBsMap[pp1.second->BB()].size();
                int succBBNum2 = _succBBsMap[pp2.second->BB()].size();
                bool isNot1 = pp1.second->customInstruction() == "CTRLNOT";
                bool isNot2 = pp2.second->customInstruction() == "CTRLNOT";
                swapOperand = (succBBNum1 < succBBNum2) || ((succBBNum1 == succBBNum2) && isNot1 && !isNot2);
            }
            if(swapOperand){
                std::swap(isBackEdge1, isBackEdge2);
                std::swap(pp1,pp2);
            }
            selNode->addInputNode(pp1.first, 0, isBackEdge1); // true data
            selNode->addInputNode(pp2.first, 1, isBackEdge2); // false data
            LLVMCDFGNode* selConNode = pp1.second;
            //add CTRLAND
            if(!hasLoopstart){    
                auto DependentNode = _BBctrlDependentNode[pp1.second->BB()];//get temsel true condition
                assert(DependentNode.size() < 2);
                LLVMCDFGNode* conditionBBCtrl = (*(DependentNode.begin())).first;
                if(DependentNode.size() == 1 && pp1.second != conditionBBCtrl){
                    LLVMCDFGNode* CTRLAND = addNode("CTRLAND", pp1.second->BB());
                    CTRLAND->addInputNode(conditionBBCtrl, 0);
                    conditionBBCtrl->addOutputNode(CTRLAND);
                    addEdge(conditionBBCtrl, CTRLAND, EDGE_TYPE_CTRL);
                    CTRLAND->addInputNode(pp1.second, 1);
                    pp1.second->addOutputNode(CTRLAND);
                    addEdge(pp1.second, CTRLAND, EDGE_TYPE_CTRL);
                    selConNode = CTRLAND;
                }
            }
            assert(selConNode != NULL);
            selNode->addInputNode(selConNode, 2); // condition
            pp1.first->addOutputNode(selNode, isBackEdge1); 
            pp2.first->addOutputNode(selNode, isBackEdge2);
            selConNode->addOutputNode(selNode);
            addEdge(pp1.first, selNode, EDGE_TYPE_DATA);
            addEdge(pp2.first, selNode, EDGE_TYPE_DATA);
            addEdge(selConNode, selNode, EDGE_TYPE_CTRL);
            if(i+2 == phiParents.size()){ // last pair, don not crete new conditional node
                // add new parent to vector
                phiParents.push_back(std::make_pair(selNode, (LLVMCDFGNode *)NULL));
            }else{
                // create new conditional node: CTRLOR (cond1 | cond2)
                LLVMCDFGNode *newCondNode = addNode("CTRLOR", node->BB());
                outs() << "new CTRLOR node = " << newCondNode->getName() << "\n";
                newCondNode->addInputNode(pp1.second, 0);
                newCondNode->addInputNode(pp2.second, 1);
                pp1.second->addOutputNode(newCondNode);
                pp2.second->addOutputNode(newCondNode);
                addEdge(pp1.second, newCondNode, EDGE_TYPE_CTRL);
                addEdge(pp2.second, newCondNode, EDGE_TYPE_CTRL);
                // add new parent to vector
                phiParents.push_back(std::make_pair(selNode, newCondNode));
            }
            //outs() << selNode->getName() << " has inputs: "<<selNode->inputNodes().size()<<"\n";
        }
        // connect last node to the successor nodes of the phi node
        LLVMCDFGNode *lastNode = phiParents.rbegin()->first;
        errs()<<"\tlastNode is: "<<lastNode->getName()<<"\n";
        errs()<<"\ttemNode is: "<<node->getName()<<"\n";
        for(auto succ : node->outputNodes()){
            bool isBackEdge = (succ->isInputBackEdge(node) | lastNode->isInputBackEdge(node));
            int idx;
            ///TODO: (code seems silly) to connect correct input idx when two same inputnodes existing
            for(int i = 0; i < succ->inputNodes().size(); i++){
                if(succ->getInputPort(i) == node){
                    idx = i;
                    break;
                }
            }
            succ->delInputNode(node);
            ///TODO: lcssa PHI isbackedge may wrong
            if(isBackEdge){
                errs()<<"\t\tlastNode to "<<succ->getName()<<" is backedge\n";
            }else{
                errs()<<"\t\tlastNode to "<<succ->getName()<<" is not backedge\n";
            }
            lastNode->addOutputNode(succ, isBackEdge);
            succ->addInputNode(lastNode, idx, isBackEdge);
            addEdge(lastNode, succ, EDGE_TYPE_DATA);
        }
        // map the PHI Node to the select node
        phi2selectMap[node] = lastNode;
        // outs() << "remove PHI node = " << node->getName() << "\n";
        // delNode(node);
    }
    // replace the unresolved PHI parent
    for(auto &elem : phi2phiParentMap){
        LLVMCDFGNode *phinode = elem.first;
        LLVMCDFGNode *phiparent = elem.second;
        LLVMCDFGNode *selparent = phi2selectMap[phiparent];
        bool isBackEdge = phinode->isInputBackEdge(phiparent); 
        // delete old connections
        int idx = phinode->delInputNode(phiparent);
        // delEdge(edge(phiparent, phinode));
        // add new connections           
        phinode->addInputNode(selparent, idx, isBackEdge);
        selparent->addOutputNode(phinode, isBackEdge);
        addEdge(selparent, phinode, EDGE_TYPE_DATA);   
    }
    // remove all phi nodes
    for(auto &elem : phi2selectMap){
        outs() << "remove PHI node = " << elem.first->getName() << "\n";
        delNode(elem.first);
    }
}


// add mask AND node behind the Shl node with bytewidth less than MAX_DATA_BYTES
void LLVMCDFG::addMaskAndNodes()
{
    std::vector<LLVMCDFGNode*> shlNodes;
    for(auto &elem : _nodes){
        auto node = elem.second;
        auto ins = node->instruction();
        if(ins && ins->getOpcode() == Instruction::Shl){
            shlNodes.push_back(node);
        }
    }
    for(auto node : shlNodes){
        auto ins = node->instruction();
        BasicBlock *BB = node->BB();
        int bytes = ins->getType()->getIntegerBitWidth() / 8;
        if(bytes < MAX_DATA_BYTES){
            LLVMCDFGNode *andNode = addNode("AND", BB);
            DataType maskVal = (1 << (8 * bytes)) - 1;
            LLVMCDFGNode *constNode = addNode("CONST", BB);
            constNode->setConstVal(maskVal);
            andNode->addInputNode(node, 0);
            andNode->addInputNode(constNode, 1);            
            constNode->addOutputNode(andNode);
            auto outNodes = node->outputNodes();
            for(auto outNode : outNodes){
                node->delOutputNode(outNode);
                int idx = outNode->delInputNode(node);
                delEdge(edge(node, outNode));
                andNode->addOutputNode(outNode);
                outNode->addInputNode(andNode, idx);
                addEdge(andNode, outNode, EDGE_TYPE_DATA);
            }
        }        
    }
}

// handle conversion operations including trunc, zext, sext
void LLVMCDFG::handleConversionNodes()
{
    for(auto &elem : _nodes){
        auto node = elem.second;
        Instruction *ins = node->instruction();
        BasicBlock *BB = node->BB();
        if(ins){
            switch (ins->getOpcode()){
                case Instruction::Trunc:{
                    TruncInst *TI = dyn_cast<TruncInst>(ins);
                    auto bitWidth = TI->getDestTy()->getIntegerBitWidth();
                    DataType mask = (DataType)(1 << bitWidth) - 1;
                    LLVMCDFGNode *constNode = addNode("CONST", BB);
                    constNode->setConstVal(mask);
                    node->addInputNode(constNode, 1);            
                    constNode->addOutputNode(node);      
                    addEdge(constNode, node, EDGE_TYPE_DATA);  
                    break;    
                }        
                case Instruction::SExt:{
                    SExtInst *SI = dyn_cast<SExtInst>(ins);
                    auto srcBitWidth = SI->getSrcTy()->getIntegerBitWidth();
                    //auto dstBitWidth = SI->getDestTy()->getIntegerBitWidth();
                    //DataType constVal = (DataType)((dstBitWidth << 8) | srcBitWidth);
                    DataType constVal = (DataType)(srcBitWidth);
                    LLVMCDFGNode *constNode = addNode("CONST", BB);
                    constNode->setConstVal(constVal);
                    node->addInputNode(constNode, 1);            
                    constNode->addOutputNode(node);      
                    addEdge(constNode, node, EDGE_TYPE_DATA);        
                    break;       
                }    
                case Instruction::ZExt:{
                    LLVMCDFGNode *constNode = addNode("CONST", BB);
                    constNode->setConstVal(0);
                    node->addInputNode(constNode, 1);            
                    constNode->addOutputNode(node);      
                    addEdge(constNode, node, EDGE_TYPE_DATA);         
                    break;
                }
                default: break;
            }
        }
    }
}

// fuse constant input node with the mul output node for acc node 
void LLVMCDFG::fuseAccConstNode()
{
    auto nodes = _nodes;
    for(auto &elem : nodes){
        auto node = elem.second;
        if(node->isAcc() && node->outputNodes().size() == 1){
            auto op0 = node->getInputPort(0);
            if(op0->customInstruction() == "CONST"){
                auto dst = node->outputNodes()[0];
                Instruction *dstIns = dst->instruction();
                auto dstCustomIns = dst->customInstruction();
                bool isMul = false;
                bool isShl = false;
                if(dstCustomIns == "MUL"){
                    isMul = true;
                }else if(dstCustomIns == "SHL"){
                    isShl = true;
                }else if(dstIns){
                    auto opc = dstIns->getOpcode();
                    if(opc == Instruction::Mul){
                        isMul = true;
                    }else if(opc == Instruction::Shl){
                        isShl = true;
                    }
                }
                if(isMul || isShl){
                    int otherIdx = 1-dst->getInputIdx(node);
                    auto otherOp = dst->getInputPort(otherIdx);
                    if(otherOp->customInstruction() == "CONST"){
                        auto otherConstVal = otherOp->constVal();
                        auto accConstVal = op0->constVal();
                        // fuse constant
                        if(isMul){
                            accConstVal *= otherConstVal;
                        }else{
                            accConstVal <<= otherConstVal;
                        }
                        op0->setConstVal(accConstVal);
                        auto dstdst = dst->outputNodes()[0];
                        // delete node and edges
                        int idx = dstdst->delInputNode(dst);
                        delNode(dst);
                        node->addOutputNode(dstdst);
                        dstdst->addInputNode(node, idx);
                        addEdge(node, dstdst, EDGE_TYPE_DATA);
                    }
                }
                
            }
        }
    }
}


// simplify the control logic
// AND/OR node connected to more than one NOT nodes
void LLVMCDFG::simplifyCtrlLogic()
{
    bool flag = true;
    while(flag){ // scan interatively
        flag = false;
        auto nodes = _nodes;
        for (auto &elem : nodes)
        {
            auto node = elem.second;
            auto BB = node->BB();
            auto customIns = node->customInstruction();
            if (customIns == "CTRLAND" || customIns == "CTRLOR")
            {
                auto op0 = node->getInputPort(0);
                auto op1 = node->getInputPort(1);
                auto dst = node->outputNodes()[0];
                bool isNotOp0 = op0->customInstruction() == "CTRLNOT";
                bool isNotOp1 = op1->customInstruction() == "CTRLNOT";
                bool isNotDst = dst->customInstruction() == "CTRLNOT";
                if (isNotOp0 + isNotOp1 + isNotDst >= 2)
                {
                    if((isNotOp0 && op0->outputNodes().size() > 1) | (isNotOp1 && op1->outputNodes().size() > 1) | (node->outputNodes().size() > 1)){
                        continue;
                    }
                    if (customIns == "CTRLAND")
                    {
                        node->setCustomInstruction("CTRLOR");
                    }
                    else
                    {
                        node->setCustomInstruction("CTRLAND");
                    }
                    if (isNotOp0)
                    {
                        auto opop = op0->getInputPort(0);
                        // delete node and edges
                        delNode(op0);
                        opop->addOutputNode(node);
                        node->addInputNode(opop, 0);
                        addEdge(opop, node, EDGE_TYPE_CTRL);
                    }
                    else
                    {
                        op0->delOutputNode(node);
                        node->delInputNode(op0);
                        delEdge(edge(op0, node));
                        LLVMCDFGNode *notNode = addNode("CTRLNOT", BB);
                        op0->addOutputNode(notNode);
                        notNode->addInputNode(op0, 0);
                        notNode->addOutputNode(node);
                        node->addInputNode(notNode, 0);
                        addEdge(op0, notNode, EDGE_TYPE_CTRL);
                        addEdge(notNode, node, EDGE_TYPE_CTRL);
                    }
                    if (isNotOp1)
                    {
                        auto opop = op1->getInputPort(0);
                        // delete node and edges
                        delNode(op1);
                        opop->addOutputNode(node);
                        node->addInputNode(opop, 1);
                        addEdge(opop, node, EDGE_TYPE_CTRL);
                    }
                    else
                    {
                        op1->delOutputNode(node);
                        node->delInputNode(op1);
                        delEdge(edge(op1, node));
                        LLVMCDFGNode *notNode = addNode("CTRLNOT", BB);
                        op1->addOutputNode(notNode);
                        notNode->addInputNode(op1, 0);
                        notNode->addOutputNode(node);
                        node->addInputNode(notNode, 1);
                        addEdge(op1, notNode, EDGE_TYPE_CTRL);
                        addEdge(notNode, node, EDGE_TYPE_CTRL);
                    }
                    if (isNotDst)
                    {
                        auto dstdst = dst->outputNodes()[0];
                        // delete node and edges
                        int idx = dstdst->delInputNode(dst);
                        delNode(dst);
                        node->addOutputNode(dstdst);
                        dstdst->addInputNode(node, idx);
                        addEdge(node, dstdst, EDGE_TYPE_CTRL);
                    }
                    else
                    {
                        node->delOutputNode(dst);
                        int idx = dst->delInputNode(node);
                        delEdge(edge(node, dst));
                        LLVMCDFGNode *notNode = addNode("CTRLNOT", BB);
                        node->addOutputNode(notNode);
                        notNode->addInputNode(node, 0);
                        notNode->addOutputNode(dst);
                        dst->addInputNode(notNode, idx);
                        addEdge(node, notNode, EDGE_TYPE_CTRL);
                        addEdge(notNode, dst, EDGE_TYPE_CTRL);
                    }
                    flag = true;
                    break;
                }
            }
        }
    }
}

// get the offset of each element in the struct
std::map<int, int> LLVMCDFG::getStructElemOffsetMap(StructType *ST)
{
    std::map<int, int> elemOffsetMap;
    int offset = 0;
    int idx = 0;
    for(Type *type : ST->elements()){
        elemOffsetMap[idx] = offset;
        offset += DL->getTypeAllocSize(type);
        idx++;
    }
    return elemOffsetMap;
}


// transfer GetElementPtr(GEP) node to MUL/ADD/Const tree
void LLVMCDFG::handleGEPNodes()
{
    std::vector<LLVMCDFGNode*> GEPNodes;
    for(auto &elem : _nodes){
        auto node = elem.second;
        auto ins = node->instruction();
        if(ins && dyn_cast<GetElementPtrInst>(ins)){
            GEPNodes.push_back(node);            
        }
    }
    for(auto node : GEPNodes){
        outs() << "----come across GEP: " << node->getName() <<"\n";
        //delete alloca node
        ///TODO: maybe need to extract alloca information
        for(auto inputNode : node->inputNodes()){
            Instruction* ins = inputNode->instruction();
            if(ins != NULL){
                if(auto allocaIns = dyn_cast<AllocaInst>(ins)){
                    errs() << "delete its alloca tree\n";
                    delNodeTree(inputNode);
                }
            }
        }
        auto ins = node->instruction();
        GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(ins);
        setGEPInfo(node, GEP->getPointerOperand()->getName().str());
        Type *currType = GEP->getSourceElementType();
        std::vector<std::pair<Value*, int>> varOperandSizes; // <non-constant-operand, element-size>
        int offset = 0;
        int NumOperands = ins->getNumOperands();
        if(NumOperands == 1){ // only one pointer, no index, so GEP should be a Const node
            // base address, should be provided by the scheduler that allocate the address space for the memory
            outs() << "GEP has only one pointer: constant base address\n";
        }else{ // have indexes, get the operand
            for(int i = 1; i < NumOperands; i++){
                outs() << "Operand " << i << ": ";
                Value *V = ins->getOperand(i);
                V->dump();
                outs() << "currType: ";
                currType->dump();
                // check if this operand is constant
                bool isConst = false;
                int constVal;
                // Instruction *I;
                if(ConstantInt *constIns = dyn_cast<ConstantInt>(V)){
                    isConst = true;
                    constVal = constIns->getSExtValue();
                    outs() << "Const: " << constVal << ", ";
                }else{
                    // I = dyn_cast<Instruction>(V);
                    outs() << "Variable, ";
                }
                // get the type and elemnt size of this operand
                int elemSize = 0;
                if(i == 1){ // source element index
                    elemSize = DL->getTypeAllocSize(currType);
                    if(isConst){
                        offset += constVal * elemSize;
                    }                   
                }else if(StructType *ST = dyn_cast<StructType>(currType)){
                    std::map<int, int> elemOffsetMap = getStructElemOffsetMap(ST);
                    assert(isConst); // the operand should be constant
                    offset += elemOffsetMap[constVal];
                    currType = ST->getElementType(constVal);
                    outs() << "StructType, ";
                }else if(ArrayType *AT = dyn_cast<ArrayType>(currType)){                    
                    currType = AT->getElementType();
                    elemSize = DL->getTypeAllocSize(currType);
                    outs() << "ArrayType, ";
                    if(isConst){
                        offset += constVal * elemSize;
                    }
                }else{
                    outs() << "Other type\n";
                    assert(false);
                }
                outs() << "\n";
                if(!isConst){
                    varOperandSizes.push_back(std::make_pair(V, elemSize));
                }
            }
        }
        // construct MUL/ADD/Const tree
        std::vector<LLVMCDFGNode*> newNodes;
        // create MUL nodes
        for(auto &elem : varOperandSizes){
            Instruction *ins = dyn_cast<Instruction>(elem.first);
            LLVMCDFGNode *preNode = this->node(ins); // predecessor node
            if(preNode == NULL){ // out of loop node
                preNode = _ioNodeMap[elem.first];
                outs() << "Out of loop ";
            }
            // elem.first->dump();
            outs() << "preNode: " << preNode->getName() << "\n";
            // delete old connection
            preNode->delOutputNode(node);
            node->delInputNode(preNode);
            delEdge(edge(preNode, node));
            // create new nodes and edges
            auto mulNode = addNode("MUL", node->BB());
            auto constNode = addNode("CONST", node->BB());
            constNode->setConstVal(elem.second);
            preNode->addOutputNode(mulNode);
            mulNode->addInputNode(preNode, 0);
            constNode->addOutputNode(mulNode);
            mulNode->addInputNode(constNode, 1);
            addEdge(preNode, mulNode, EDGE_TYPE_DATA);
            addEdge(constNode, mulNode, EDGE_TYPE_DATA);
            newNodes.push_back(mulNode);
        }
        // create ADD nodes
        for(int i = 0; i + 1 < newNodes.size(); i += 2){
            auto n1 = newNodes[i];
            auto n2 = newNodes[i+1];
            auto newNode = addNode("ADD", node->BB());
            n1->addOutputNode(newNode);
            n2->addOutputNode(newNode);
            newNode->addInputNode(n1, 0);
            newNode->addInputNode(n2, 1);
            addEdge(n1, newNode, EDGE_TYPE_DATA);
            addEdge(n2, newNode, EDGE_TYPE_DATA);
            newNodes.push_back(newNode);
        }
        // connect the last node to GEP node and set constant
        outs() << "Total offset: " << offset << "\n";
        node->setConstVal(offset);
        if(!newNodes.empty()){
            auto lastNewNode = *(newNodes.rbegin());
            lastNewNode->addOutputNode(node);
            node->addInputNode(lastNewNode, 0);
            addEdge(lastNewNode, node, EDGE_TYPE_DATA);
        }
        // the flollowing solution will delete GEP node
        LLVMCDFGNode* GEPAdd = NULL;
        if(node->inputNodes().size() == 2){
            GEPAdd = addNode("ADD", node->BB());
            int idx = 0;
            for(auto inputNode : node->inputNodes()){
                inputNode->addOutputNode(GEPAdd);
                errs() << "inputNode "<< inputNode->getName() << "is port "<< node->getInputIdx(inputNode) << " of "<< node->getName() <<"\n";
                GEPAdd->addInputNode(inputNode, idx);
                addEdge(inputNode, GEPAdd, EDGE_TYPE_DATA);
                idx++;
            }
        }else if(node->inputNodes().size() == 1){
            GEPAdd = *(node->inputNodes().begin());
        }else if(node->inputNodes().size() == 0){
            GEPAdd = addNode("CONST", node->BB());
            GEPAdd->setConstVal(node->constVal());
            errs() << "const address access\n";
        }else{
            assert(0);
        }
        int LSoffset = 0;
        if(node->constVal() != 0){///add const offset of GEP
            LSoffset = node->constVal();
        }
        //connect output of GEP to GEPAdd
        assert(GEPAdd != NULL);
        for(auto outputNode : node->outputNodes()){
            auto ins = outputNode->instruction();
            if(dyn_cast<LoadInst>(ins) || dyn_cast<StoreInst>(ins)){
                outputNode->setLSoffset(node->constVal());
            }
            outputNode->addInputNode(GEPAdd, outputNode->delInputNode(node));
            GEPAdd->addOutputNode(outputNode);
            addEdge(GEPAdd, outputNode, EDGE_TYPE_DATA);
        }
        delNode(node);
    }
}


// add loop exit nodes
void LLVMCDFG::addLoopExitNodes()
{
    for(auto &elem : _loopexitBBs){
        BasicBlock *srcBB = elem.first;
        BasicBlock *dstBB = elem.second;        
        LLVMCDFGNode *ctrlNode;
        bool isTrueCond;
        BranchInst *BRI = cast<BranchInst>(srcBB->getTerminator());
        if(BRI->isConditional()){
            ctrlNode = node(dyn_cast<Instruction>(BRI->getCondition()));
            // check true/false condition
            for(int i = 0; i < BRI->getNumSuccessors(); i++){
                if(dstBB == BRI->getSuccessor(i)){
                    isTrueCond = (i == 0);
                    break;
                }
            }
        }else{ // find the control node in the dominating BB
            BasicBlock *domBB = DT->getNode(srcBB)->getIDom()->getBlock();
            BranchInst *domBRI = cast<BranchInst>(domBB->getTerminator());
            assert(domBRI->isConditional());
            ctrlNode = node(dyn_cast<Instruction>(domBRI->getCondition()));
            // check true/false condition
            for(int i = 0; i < domBRI->getNumSuccessors(); i++){
                if(DT->dominates(domBRI->getSuccessor(i), srcBB)){
                    isTrueCond = (i == 0);
                    break;
                }
            }
        }        
        // create LOOPEXIT node and connect ctrlNode to it
        auto exitNode = getLoopExitNode(srcBB);
        if(!isTrueCond){
            // find the CTRLNOT node
            for(auto outNode : ctrlNode->outputNodes()){
                if(outNode->customInstruction() == "CTRLNOT"){
                    ctrlNode = outNode;
                    break;
                }
            }
        }
        outs() << "Control node: " << ctrlNode->getName();
        outs() << ", exit node: " << exitNode->getName() << "\n";
        exitNode->addInputNode(ctrlNode, 0);
        ctrlNode->addOutputNode(exitNode);
        addEdge(ctrlNode, exitNode, EDGE_TYPE_CTRL);
    }
}


// remove redundant nodes, e.g. Branch
void LLVMCDFG::removeRedundantNodes()
{
    auto nodes = _nodes;
    for(auto &elem : nodes){
        auto node = elem.second;
        Instruction *ins = node->instruction();
        std::string customIns = node->customInstruction();
        if(node->isConditional()){ // is br nodes
            delNodeTree(node);// to delete BrNode & all of its parents nodes recursively
        }
    }
    // remove redundant nodes iteratively until no node to be remove
    bool removed = true;
    while(removed){
        std::vector<LLVMCDFGNode*> rmNodes;
        for(auto &elem : _nodes){
            auto node = elem.second;
            Instruction *ins = node->instruction();
            std::string customIns = node->customInstruction();
            if(node->inputNodes().empty()){ // no input node
                if(!(ins && dyn_cast<LoadInst>(ins)) && customIns != "CONST" && customIns != "LOOPSTART" && customIns != "INPUT" && customIns != "ArgIn"){ //  && !node->isLSaffine()
                    rmNodes.push_back(node);
                    continue;
                }
            } 
            if(node->outputNodes().empty()){ // no output node
                if(!(ins && dyn_cast<StoreInst>(ins)) && customIns != "LOOPEXIT" && customIns != "OUTPUT"){ //  && !node->isLSaffine()
                    rmNodes.push_back(node);
                    continue;
                }
            }
            if(node->isLSaffine() && customIns != "OUTPUT"){ // for temp test, to be removed later
                bool toRm = true;
                auto strides = node->getLSstride();
                for(auto &elem : strides){  // strides are all 0
                    if(elem.second != 0){
                        toRm = false;
                        break;
                    }
                }
                if(toRm){
                    rmNodes.push_back(node);
                    continue;
                }
            }
        }
        removed = !rmNodes.empty();
        outs() << "remove node: ";
        for(auto node : rmNodes){
            outs() << node->getName() << ", \n\t";
            delNode(node);            
        }
        outs() << "\n";
    }
}


// assign final node name
void LLVMCDFG::assignFinalNodeName()
{
    for(auto &elem : _nodes){
        auto node = elem.second;
        Instruction *ins = node->instruction();
        std::string customIns = node->customInstruction();
        if(!customIns.empty()){
            if(customIns == "SELECT"){
                customIns = "SEL";
            }else if(customIns == "InitSELECT"){
                customIns = "ISEL";
            }else if(customIns.size() > 4 && customIns.substr(0, 4) == "CTRL"){
                customIns = customIns.substr(4);
            }
            node->setFinalInstruction(customIns);
        }else{
            switch (ins->getOpcode()){
            case Instruction::Add:
                node->setFinalInstruction("ADD");
                break;
            case Instruction::FAdd:
                node->setFinalInstruction("FADD");
                break;
            case Instruction::Sub:
                node->setFinalInstruction("SUB");
                break;
            case Instruction::FSub:
                node->setFinalInstruction("FSUB");
                break;
            case Instruction::Mul:
                node->setFinalInstruction("MUL");
                break;
            case Instruction::FMul:
                node->setFinalInstruction("FMUL");
                break;
            case Instruction::UDiv:
            case Instruction::SDiv:
            case Instruction::FDiv:
                // errs() << "Do not support Div Instructions\n";
                // assert(false);
                // break;
                node->setFinalInstruction("DIV");
                break;
            case Instruction::URem:
            case Instruction::SRem:
            case Instruction::FRem:
                errs() << "Do not support Rem Instructions\n";
                assert(false);
                break;
            case Instruction::Shl:
                node->setFinalInstruction("SHL");
                break;
            case Instruction::LShr:
                node->setFinalInstruction("LSHR");
                break;
            case Instruction::AShr:
                node->setFinalInstruction("ASHR");
                break;
            case Instruction::And:
                node->setFinalInstruction("AND");
                break;
            case Instruction::Or:
                node->setFinalInstruction("OR");
                break;
            case Instruction::Xor:
                node->setFinalInstruction("XOR");
                break;
            case Instruction::Load:
                if(node->isLSaffine())
                    node->setFinalInstruction("Input");
                else{
                    if(node->getInputPort(2)!=NULL)
                        node->setFinalInstruction("CLOAD");
                    else
                        node->setFinalInstruction("LOAD");
                }
                break;
            case Instruction::Store:
                if(node->isLSaffine())
                    node->setFinalInstruction("Output");
                else{
                    if(node->getInputPort(2)!=NULL)
                        node->setFinalInstruction("CSTORE");
                    else
                        node->setFinalInstruction("STORE");
                }
                break;
            case Instruction::GetElementPtr:
                if(node->inputNodes().empty()){
                    node->setFinalInstruction("CONST");
                }else{
                    node->setFinalInstruction("ADD");
                }                
                break;
            case Instruction::Trunc:{
                // TruncInst *TI = dyn_cast<TruncInst>(ins);
                // auto bitWidth = TI->getDestTy()->getIntegerBitWidth();
                // DataType mask = (DataType)(1 << bitWidth) - 1;
                // node->setConstVal(mask);
                node->setFinalInstruction("AND");             
                break;    
            }        
            case Instruction::SExt:{
                // SExtInst *SI = dyn_cast<SExtInst>(ins);
                // auto srcBitWidth = SI->getSrcTy()->getIntegerBitWidth();
                // auto dstBitWidth = SI->getDestTy()->getIntegerBitWidth();
                // DataType constVal = (DataType)((dstBitWidth << 8) | srcBitWidth);
                // node->setConstVal(constVal);
                node->setFinalInstruction("SEXT");         
                break;       
            }    
            case Instruction::ZExt:{
                //node->setConstVal(0);
                node->setFinalInstruction("OR");         
                break;
            }
            case Instruction::ICmp:{
                CmpInst *CI = dyn_cast<CmpInst>(ins);
                switch(CI->getPredicate()){
                case CmpInst::ICMP_EQ:
                    node->setFinalInstruction("EQ");
                    break;
                case CmpInst::ICMP_NE:
                    node->setFinalInstruction("NE");
                    break;
                case CmpInst::ICMP_SGE:{
                    //node->setFinalInstruction("SGE");
                    node->setFinalInstruction("SLE");
                    LLVMCDFGNode* opnode0 = node->getInputPort(0), *opnode1 = node->getInputPort(1);
                    node->setInputIdx(opnode0, 1);
                    node->setInputIdx(opnode1, 0);
                    break;
                }
                case CmpInst::ICMP_UGE:{
                    //node->setFinalInstruction("UGE");
                    node->setFinalInstruction("ULE");
                    LLVMCDFGNode* opnode0 = node->getInputPort(0), *opnode1 = node->getInputPort(1);
                    node->setInputIdx(opnode0, 1);
                    node->setInputIdx(opnode1, 0);
                    break;
                }
                case CmpInst::ICMP_SGT:{
                    //node->setFinalInstruction("SGT");
                    node->setFinalInstruction("SLT");
                    LLVMCDFGNode* opnode0 = node->getInputPort(0), *opnode1 = node->getInputPort(1);
                    node->setInputIdx(opnode0, 1);
                    node->setInputIdx(opnode1, 0);
                    break;
                }
                case CmpInst::ICMP_UGT:{
                    //node->setFinalInstruction("UGT");
                    node->setFinalInstruction("ULT");
                    LLVMCDFGNode* opnode0 = node->getInputPort(0), *opnode1 = node->getInputPort(1);
                    node->setInputIdx(opnode0, 1);
                    node->setInputIdx(opnode1, 0);
                    break;
                }
                case CmpInst::ICMP_SLE:
                    node->setFinalInstruction("SLE");
                    break;
                case CmpInst::ICMP_ULE:
                    node->setFinalInstruction("ULE");
                    break;
                case CmpInst::ICMP_SLT:
                    node->setFinalInstruction("SLT");
                    break;
                case CmpInst::ICMP_ULT:
                    node->setFinalInstruction("ULT");
                    break;
                default:
                    assert(false);
                    break;
                }
                break;
            }
            case Instruction::Select:
                node->setFinalInstruction("SEL");
                break;
            case Instruction::PHI:
                //node->setFinalInstruction("SELECT");
                break;
            default:
                ins->dump();
                assert(false);
                break;
            }
        }
    }
}

///to cast constant value to Int
int castConstInt(Constant* V){
    int integer;
    if (ConstantInt *CI = dyn_cast<ConstantInt>(V))
    {
        integer = CI->getSExtValue();
    }
    else if (ConstantFP *FP = dyn_cast<ConstantFP>(V))
    {
        integer = (int)FP->getValueAPF().convertToFloat();
    }
    else{
        assert(0&&"fail in casting const value to Int");
    }
    return integer;
}

///to get the constant operand of a BinaryOperation(first:OperandIndex; second:IntValue)
std::pair<int, int> BinaryConstOp(Instruction *Bi)
{
    int OperandIndex;
    int constOp;
    assert(Bi && (Bi->getNumOperands() == 2));
    if (auto V = dyn_cast<Constant>(Bi->getOperand(0)))
    {
        constOp = castConstInt(V);
        OperandIndex = 0;
        //errs() << "\t" << "Instructions: " << Bi->getName().str() << "\n";
    }
    else if (auto V = dyn_cast<Constant>(Bi->getOperand(1)))
    {
        constOp = castConstInt(V);
        OperandIndex = 1;
        //errs() << "\t" << "Instructions: " << Bi->getName().str() << "\n";
    }else{
        return std::make_pair(-1, 0);
        assert(0&&"do not have Constant");
    }
    return std::make_pair(OperandIndex, constOp);
}

///to get the constant operand of a BinaryOperation(first:OperandIndex; second:IntValue)
std::pair<int, int> BinaryConstOp(LLVMCDFGNode* Bn)
{
    int OperandIndex;
    int constOp;
    assert(Bn && (Bn->inputNodes().size() == 2));
    if (Bn->getInputPort(0)->hasConst())
    {
        constOp = Bn->getInputPort(0)->constVal();
        OperandIndex = 0;
        //errs() << "\t" << "Instructions: " << Bi->getName().str() << "\n";
    }
    else if (Bn->getInputPort(1)->hasConst())
    {
        constOp = Bn->getInputPort(1)->constVal();
        OperandIndex = 1;
        //errs() << "\t" << "Instructions: " << Bi->getName().str() << "\n";
    }else{
        return std::make_pair(-1, 0);
        assert(0&&"do not have Constant");
    }
    return std::make_pair(OperandIndex, constOp);
}

LLVMCDFGNode* LLVMCDFG::addNodeTree(Value* opnode){
    for (auto iter : _funcArgs)
    {
        auto arg = iter.second;
        if (arg == opnode)
        {
            LLVMCDFGNode* newnode = addNode("ArgIn");
            return newnode;
        }
    }
    if(auto CT = dyn_cast<Constant>(opnode)){
        LLVMCDFGNode* newnode = addNode("CONST");
        newnode->setConstVal(castConstInt(CT));
        return newnode;
    //PHI will be handled in arraystride()
    }else if(auto Phi = dyn_cast<PHINode>(opnode)){
        //LLVMCDFGNode *newnode = addNode("SELECT", Phi->getParent());
        return node(Phi);
    }else if(auto Load = dyn_cast<LoadInst>(opnode)){
        LLVMCDFGNode *newnode = addNode(Load);
        if(dyn_cast<GetElementPtrInst>(Load->getOperand(0))){
            LLVMCDFGNode* prenode = addNodeTree(Load->getOperand(0));
            prenode->addOutputNode(newnode);
            newnode->addInputNode(prenode, 0);
            addEdge(prenode, newnode, EDGE_TYPE_DATA);
        }else{
            ///TODO: is it right?
            newnode->setCustomInstruction("INREG");
        }
        return newnode;
    }else if(auto GEP = dyn_cast<GetElementPtrInst>(opnode)){
        LLVMCDFGNode *newnode = addNode(GEP);
        int dimension = GEP->getNumIndices();
        auto teminputs = newnode->inputNodes();
        for(auto prenode : teminputs){
            if(prenode->customInstruction() == "INPUT"){
                delNode(prenode);
            }
        }
        for(int i = 2; i <= dimension; i++){
            auto operand = GEP->getOperand(i);
            LLVMCDFGNode* prenode;
            if(auto Phi = dyn_cast<PHINode>(operand)){//PHI direct connnect to GEP handle here
                auto inputs = newnode->inputNodes();
                prenode = addIdxCycle(Phi);
                if(prenode->customInstruction().empty() && _loopBBs.count(Phi->getParent()))///innermost phi
                    continue;
            }else if(dyn_cast<Constant>(operand)){//handled in handleGEP
                continue;
            }else{
                prenode = addNodeTree(operand);
            }
            if(newnode->getInputPort(i) == prenode)//have been added
                continue;
            prenode->addOutputNode(newnode);
            newnode->addInputNode(prenode, i);
            addEdge(prenode, newnode, EDGE_TYPE_DATA);
        }
        return newnode;
    }else if (auto ins = dyn_cast<Instruction>(opnode))//TODO:outter action in DFG may be wrong
    {
        LLVMCDFGNode *newnode = node(ins);
        if(newnode != NULL)
            return newnode;
        else
            newnode = addNode(ins);
        for(int i = 0; i < ins->getNumOperands(); i++){
            auto operand = ins->getOperand(i);
            if(dyn_cast<PHINode>(operand))//get PHI operand.deal in arraystride
                continue;
            LLVMCDFGNode* prenode = addNodeTree(operand);
            prenode->addOutputNode(newnode);
            newnode->addInputNode(prenode, i);
            addEdge(prenode, newnode, EDGE_TYPE_DATA);
        }
        return newnode;
    }else{
        return NULL;
    }
}

std::set<std::pair<LLVMCDFGNode*, int>> LLVMCDFG::addOutputTree(Value* user){
    std::set<std::pair<LLVMCDFGNode*, int>> resultset;
    auto ins = dyn_cast<Instruction>(user);
    for(auto succ : user->users()){
        if(auto succIns = dyn_cast<Instruction>(succ)){
            if(_loopBBs.count(succIns->getParent())){
                continue;
            }else if(auto Phi = dyn_cast<PHINode>(succIns)){//single PHI is just pass
                if(Phi->getNumOperands() != 1)///only consider lcssa new PHI:TODO ?
                    continue;
                errs() << "find phi:";Phi->dump();
                auto outputset = addOutputTree(Phi);
                resultset.insert(outputset.begin(), outputset.end());
            }else if(auto Store = dyn_cast<StoreInst>(succIns)){
                errs() << "find store:";Store->dump();
                int idx;
                for(idx = 0; idx<Store->getNumOperands(); idx++){
                    if(auto operandins = dyn_cast<Instruction>(Store->getOperand(idx))){
                        {if(operandins == ins)
                            break;
                        }
                    }
                }
                resultset.insert(std::make_pair(addNode(Store), idx));
            }else if(dyn_cast<BranchInst>(succIns)||dyn_cast<ReturnInst>(succIns)){
                succIns->dump();///TODO:can not handle like:ret...
                continue;
            }else{
                succIns->dump();
                auto detectedflag = (node(succIns) != NULL);
                auto succnode = addNode(succIns);
                int idx = -1;
                for(int i = 0; i < succIns->getNumOperands(); i++){
                    if(auto operandins = dyn_cast<Instruction>(succIns->getOperand(i))){
                        if(operandins == ins){
                            idx = i;
                            continue;
                        }else if(dyn_cast<PHINode>(operandins)){
                            continue;
                        }else if(operandins != ins && node(operandins) == NULL){
                            auto opnode = addNodeTree(operandins);
                            opnode->addOutputNode(succnode);
                            succnode->addInputNode(opnode);
                            addEdge(opnode, succnode, EDGE_TYPE_DATA);
                            succnode->setInputIdx(opnode, i);
                        }
                    }
                    else if(dyn_cast<Constant>(succIns->getOperand(i))){
                        if(!detectedflag){
                            auto opnode = addNodeTree(succIns->getOperand(i));
                            opnode->addOutputNode(succnode);
                            succnode->addInputNode(opnode);
                            addEdge(opnode, succnode, EDGE_TYPE_DATA);
                            succnode->setInputIdx(opnode, i);
                        }
                    }else{
                        assert(0&&"what's wrong");
                    }
                }
                resultset.insert(std::make_pair(succnode, idx));
                auto outputset = addOutputTree(succIns);
                for(auto temoutput:outputset){
                    Instruction* temoutputins = temoutput.first->instruction();
                    int idx=-1;
                    for(auto i = 0; i<temoutputins->getNumOperands(); i++){
                        if(temoutputins->getOperand(i) == succIns){
                            idx = i;
                            break;
                        }
                    }
                    assert(idx>=0);
                    if(edge(succnode, temoutput.first)==NULL || temoutput.first->getInputIdx(succnode) != idx){
                        succnode->addOutputNode(temoutput.first);
                        temoutput.first->addInputNode(succnode);
                        addEdge(succnode, temoutput.first, EDGE_TYPE_DATA);
                        temoutput.first->setInputIdx(succnode, idx);
                    }
                }
            }
        }else{
            succ->dump();
            assert(0&&"come assross what in add OutputTree");
            return resultset;
        }
    }
    return resultset;
}

LLVMCDFGNode* LLVMCDFG::addIdxCycle(PHINode* Phi){
    if(Phi->getNumIncomingValues() > 2){
        assert(0&&"what phi =_=");
    }
    auto exist = node(Phi);
    if(exist != NULL)
        return exist;
    LLVMCDFGNode* SELECT = addNode(Phi);
    SELECT->setCustomInstruction("SELECT");
    for(int i = 0; i < Phi->getNumIncomingValues(); i++){
        BasicBlock* bb = Phi->getIncomingBlock(i);
		Value* V = Phi->getIncomingValue(i);
		outs() << "IncomingValue :: "; V->dump();
        if(auto C = dyn_cast<Constant>(V)){///TODO:very puny constrain
            LLVMCDFGNode* phiTree = addNodeTree(V);
            SELECT->addInputNode(phiTree);
            SELECT->setInputIdx(phiTree, i);
            phiTree->addOutputNode(SELECT);
            addEdge(phiTree, SELECT, EDGE_TYPE_DATA);
        }else if(auto ins = dyn_cast<Instruction>(V)){
            LLVMCDFGNode* phiTree = node(ins);
            if(phiTree == NULL)
                phiTree = addNodeTree(V);
            SELECT->addInputNode(phiTree);
            SELECT->setInputIdx(phiTree, i);
            phiTree->addOutputNode(SELECT);
            auto phiTreeins = phiTree->instruction();
            int Sel2phiTreeidx = -1;
            for(int i = 0; i < phiTreeins->getNumOperands(); i++){
                if(phiTreeins->getOperand(i) == Phi){
                    Sel2phiTreeidx = i;
                    break;
                }
            }
            assert(Sel2phiTreeidx != -1);
            phiTree->setInputIdx(SELECT, Sel2phiTreeidx);
            addEdge(phiTree, SELECT, EDGE_TYPE_DATA);
            addEdge(SELECT, phiTree, EDGE_TYPE_DATA);
            phiTree->setOutputBackEdge(SELECT, true);
            
        }
    }
    return SELECT;
}


PHINode* InstoPHI(Instruction* src){
    for (int i = 0; i < src->getNumOperands(); i++){
        if(auto ins = dyn_cast<Instruction>(src->getOperand(i))){
            if (auto Phi = dyn_cast<PHINode>(ins)){
                return Phi;
            }
            else{
                return InstoPHI(ins);
            }
        }
        else{
            continue;
        }
    }
    return NULL;
}

///find safety branch of variable Idx and modify dependent node
void LLVMCDFG::FindLoopIdxSafetyBr(int level, Value* bound){
    Loop* temloop = nestloops()[level];
    BasicBlock* header = temloop->getHeader();
    auto ctrlNodes = _BBctrlDependentNode[header];
    //assert(!ctrlNodes.empty());///the outter most header may have no CtrlNodes
    for(auto iter : ctrlNodes){
        bool findbound = false;
        Instruction* safetycmp =  iter.first->instruction();
        assert(safetycmp != NULL);
        assert(dyn_cast<CmpInst>(safetycmp));
        for(int i = 0; i < safetycmp->getNumOperands(); i++){
            if(bound == safetycmp->getOperand(i)){
                findbound = true;
                _safetyMap[level] = std::make_pair(dyn_cast<CmpInst>(safetycmp), iter.second);
                break;
            }
        }
        if(findbound){
            _BBctrlDependentNode[header].erase(iter);
        }
    }
    outs() << "Find LoopIdx Safety of level "<< level <<"\n";
    outs() << "header "<< header->getName() << "'s final ctrlNodes are ";
    for(auto iter : _BBctrlDependentNode[header]){
        outs() << iter.first->getName() << ", " << iter.second << ";";
    }
    outs() << "\n";
}

///analyze loop index
void LLVMCDFG::LoopIdxAnalyze(){
    errs() << "----------------analyze Loop Idx\n";
    _nestLoopisAffine = true;
    std::map<int, llvm::Loop *> nestloops = _nestloops;
    int total_level = nestloops.size();
    std::set<BasicBlock*> detected; //detected loops' BBs shouldn't be checked by outter loop
    std::map<int, std::set<BasicBlock*>> LoopexclusiveBBs;
    std::map<int, int> strides;
    //std::map<int, std::pair<int, int>> bounds;
    std::map<int, std::pair<varType, varType>> bounds;

    bounds.clear();
    strides.clear();
    
//get every nests' idx stride & bound
    for(int level = 0; level < total_level; level++){
        ///get all basicblocks of this loop nest
        std::set<BasicBlock*> temNestBBs;
        temNestBBs.insert(nestloops[level]->getBlocks().begin(), nestloops[level]->getBlocks().end());
        Instruction* idx_gen_oprand;
        Value* indexbegin, *indexend;
        int stride,leftbound,rightbound;
        BasicBlock* exitBB = nestloops[level]->getExitBlock();
        Instruction* exitins = exitBB->getTerminator();
        auto branch = dyn_cast<BranchInst>(exitins);
        CmpInst* exitcmp;
        PHINode* idxPhi = NULL;
        //maybe outmost loop
        if(!temNestBBs.count(exitBB)){
            exitBB = exitBB->getSinglePredecessor();
            assert(exitBB&&"how does it exit?");
            exitins = exitBB->getTerminator();
            branch = dyn_cast<BranchInst>(exitins);
        }

        errs() << "nest: " << level << " have blocks: ";
        for(auto BI = temNestBBs.begin(); BI != temNestBBs.end(); BI++){
            BasicBlock *temBB = *BI;
            errs() << temBB->getName().str() << " ";
        }
        
        errs() << "\n"; 
        errs() << "exit block is: " << exitBB->getName().str() <<"\n";
        errs() << "exit branch is: "; branch->dump();
        //errs() << "Canonical Induction Variable is: ";nestloops[level]->getCanonicalInductionVariable()->dump();


        if(auto cmp = dyn_cast<CmpInst>(branch->getOperand(0))){
            exitcmp = cmp;
            errs() << "exit cmp is: "; exitcmp->dump();
        }

        idxPhi = InstoPHI(exitcmp);
        if(!nestloops[level]->getBounds(*SE).hasValue()){
            continue;
        }
        //errs() << "getBoundFuntion: level step: "<< nestloops[level]->getBounds(*SE).getPointer()->getStepInst().getName().str() << "\n";
        if (auto LoopBound = (nestloops[level]->getBounds(*SE).getPointer()))
        {   
            Instruction *idx_gen_oprand = &(LoopBound->getStepInst());
            Value* InitialIVValue, *FinalIVValue;
            InitialIVValue = (&(LoopBound->getInitialIVValue()));
            FinalIVValue = (&(LoopBound->getFinalIVValue()));
            auto tem2 = nestloops[level];
            //stride
            if(idx_gen_oprand->getOpcode() != Instruction::Add && idx_gen_oprand->getOpcode() != Instruction::Sub){
                errs() << "\t" << idxPhi->getName().str() << " had non-affine operation\n";
                this->_nestLoopisAffine = false;
            }else{
                if (auto V = dyn_cast<Constant>(idx_gen_oprand->getOperand(0)))
                {
                    int stride;
                    if (ConstantInt *CI = dyn_cast<ConstantInt>(V))
                    {
                        stride = CI->getSExtValue();
                    }
                    else if (ConstantFP *FP = dyn_cast<ConstantFP>(V))
                    {
                        stride = (int)FP->getValueAPF().convertToFloat();
                    }
                    errs() << "\t"
                        << "Instructions: " << idx_gen_oprand->getName().str() << "\n";
                    strides[level] = stride;
                }
                else if (auto V = dyn_cast<ConstantInt>(idx_gen_oprand->getOperand(1)))
                {
                    int stride;
                    if (ConstantInt *CI = dyn_cast<ConstantInt>(V))
                    {
                        stride = CI->getSExtValue();
                    }
                    else if (ConstantFP *FP = dyn_cast<ConstantFP>(V))
                    {
                        stride = (int)FP->getValueAPF().convertToFloat();
                    }
                    errs() << "\t"
                        << "Instructions: " << idx_gen_oprand->getName().str() << "\n";
                    strides[level] = stride;
                }
                else{
                    errs() << "\t" << idxPhi->getName().str() << " have affain operation, but no constant operands\n";
                }
            }
            //bound
            if(auto CV = dyn_cast<ConstantInt>(InitialIVValue)){
                bounds[level].first = castConstInt(CV);
            }else{
                errs() << "\tinitial value is not ConstantInt\n";
            }
            if(auto CV = dyn_cast<ConstantInt>(FinalIVValue)){
                bounds[level].second = castConstInt(CV);
            }else{
                errs() << "\tfinal value is not ConstantInt\n";
                bool findArgIn = false;
                for(auto iter : _funcArgs){
                    auto arg = iter.second;
                    if(arg == FinalIVValue){
                        errs() << "\tFinalIVValue is the "<< iter.first << " input param of this functuion\n";
                        FindLoopIdxSafetyBr(level, FinalIVValue);
                        bounds[level].second = "arg" + std::to_string(iter.first);
                        findArgIn = true;
                        break;
                    }
                }
                if(!findArgIn){
                    _nestLoopisAffine = false;
                }
            }
            if(exitcmp->getOperand(0) == idxPhi){
                if(bounds[level].second.index() == 0){
                    bounds[level].second = strides[level] + bounds[level].second;
                }else{
                    assert(false&&"complete");
                }
            }else if(auto truenext = branch->getSuccessor(0)){
                errs()<<idxPhi->getName().str()<<" is in "<<idxPhi->getParent()->getName().str()<<"\n";
                errs()<<"true next is "<<truenext->getName().str()<<"\n";
                if(truenext == idxPhi->getParent()){///cmp is true, but loop repeat
                    bounds[level].second += strides[level];///TODO: all right?
                    errs() << "\t"<< exitcmp->getName().str()<<" is true, but loop repeat; bounds[level].second = "<<bounds[level].second.to_string() << "\n";
                }
            }
        }else{
            errs() << "\t" <<  " had no LoopBound pointer\n";
            this->_nestLoopisAffine = false;
        }


        for(auto BI = temNestBBs.begin(); BI != temNestBBs.end(); BI++){
            BasicBlock *temBB = *BI;
            if (detected.find(temBB) == detected.end())
            {
                LoopexclusiveBBs[level].insert(temBB);
                for (auto II = temBB->begin(); II != temBB->end(); II++)
                {
                    Instruction *ins = &*II;
                    if (auto Phi = dyn_cast<PHINode>(ins))
                    {
                        if(Phi != idxPhi){
                            continue;
                        }
                        PHItoLevel[Phi] = level;
                    }
                }
            }
            /*
                for (auto II = temBB->begin(); II != temBB->end(); II++)
                {
                    Instruction *ins = &*II;
                    if (auto Phi = dyn_cast<PHINode>(ins))
                    {
                        if(Phi != idxPhi){
                            continue;
                        }
                        PHItoLevel[Phi] = level;
                        BasicBlock *PhiNext;
                        for (int i = 0; i < Phi->getNumOperands(); i++)
                        {
                            PhiNext = Phi->getIncomingBlock(i);
                            errs() << "\tPHINode: " << Phi->getName().str() << " getIncomingBlock(" << i << ") is: " << PhiNext->getName().str() << "\n";
                            if (temNestBBs.find(PhiNext) != temNestBBs.end())
                            {
                                idx_gen_oprand = dyn_cast<Instruction>(Phi->getIncomingValueForBlock(PhiNext)); // PROBLEM
                                if (dyn_cast<SExtInst>(idx_gen_oprand))
                                {
                                    idx_gen_oprand = dyn_cast<Instruction>(idx_gen_oprand->getOperand(0));
                                }
                                if (idx_gen_oprand->getNumOperands() != 2)
                                { // to avoid idx_gen_operand is somelike GEP
                                    continue;
                                }
                                bool affineflag = true;
                                int  stride = 0;
                                Value *op0, *op1;
                                while(op0 != Phi && op1 != Phi)
                                {
                                    if(idx_gen_oprand->getOpcode() != Instruction::Add && idx_gen_oprand->getOpcode() != Instruction::Sub){
                                        affineflag = false;
                                        break;
                                    }
                                    op0 = idx_gen_oprand->getOperand(0);
                                    op1 = idx_gen_oprand->getOperand(1);
                                    if (auto V = dyn_cast<Constant>(op0))
                                    {
                                        stride += castConstInt(V);
                                        errs() << "\t"
                                               << "Instructions: " << idx_gen_oprand->getName().str() << "\n";
                                        //strides[level] = stride;
                                        if(auto next = dyn_cast<Instruction>(idx_gen_oprand->getOperand(1)))
                                            idx_gen_oprand = next;
                                    }
                                    else if (auto V = dyn_cast<ConstantInt>(op1))
                                    {
                                        stride += castConstInt(V);
                                        errs() << "\t"
                                               << "Instructions: " << idx_gen_oprand->getName().str() << "\n";
                                        //strides[level] = stride;
                                        if(auto next = dyn_cast<Instruction>(idx_gen_oprand->getOperand(0)))
                                            idx_gen_oprand = next;
                                    }
                                    else
                                    { // index operation has non-constant oprand
                                        errs() << "\t" << Phi->getName().str() << " had affain operation but the Operand is not Const\n";
                                        this->_nestLoopisAffine = false;
                                        break;
                                    }
                                }
                                if(affineflag){
                                    strides[level] = stride;
                                }else{
                                    errs() << "\t" << Phi->getName().str() << " had non-affain operation\n";
                                    this->_nestLoopisAffine = false;
                                }
                                if (isloopsAffine())
                                {
                                    bool flag = true; // flag false means: tem phi control branch
                                    for (auto phiusers : Phi->users())
                                    {
                                        if (auto I = dyn_cast<Instruction>(phiusers))
                                        {
                                            // errs() << "PHI node users: " << I->getName().str() << "\n";
                                            if (auto cmp = dyn_cast<ICmpInst>(I))
                                            {
                                                if(cmp != exitcmp)
                                                    continue;
                                                errs() << "\tcmp node is: " << cmp->getName().str() << "\n";
                                                flag = false;
                                                bounds[level].second = BinaryConstOp(I).second + stride;
                                                //errs() << "\twhy: bounds[level].second = "<<bounds[level].second << "\n";
                                                if(auto truenext = branch->getSuccessor(0)){
                                                    errs()<<Phi->getName().str()<<" is in "<<Phi->getParent()->getName().str()<<"\n";
                                                    errs()<<"true next is "<<truenext->getName().str()<<"\n";
                                                    if(truenext == Phi->getParent()){///cmp is true, but loop repeat
                                                        bounds[level].second = BinaryConstOp(I).second + stride + stride;
                                                        errs() << "\t"<< cmp->getName().str()<<" is true, but loop repeat; bounds[level].second = "<<bounds[level].second << "\n";
                                                    }
                                                } 
                                            }
                                        }
                                    }
                                    if (flag)//phi do not control CMP directly
                                    {
                                        for (auto opusers : Phi->getIncomingValueForBlock(PhiNext)->users())
                                        {
                                            if (auto I = dyn_cast<Instruction>(opusers))
                                            {
                                                if (auto cmp = dyn_cast<ICmpInst>(I))
                                                {
                                                    if(cmp != exitcmp)
                                                        continue;
                                                    errs() << "\tcmp node is: " << cmp->getName().str() << "\n";
                                                    bounds[level].second = BinaryConstOp(I).second;
                                                    if(auto truenext = branch->getSuccessor(0)){
                                                        errs()<<Phi->getName().str()<<" is in "<<Phi->getParent()->getName().str()<<"\n";
                                                        errs()<<"true next is "<<truenext->getName().str()<<"\n";
                                                        if(truenext == Phi->getParent()){///cmp is true, but loop repeat
                                                            bounds[level].second = BinaryConstOp(I).second + stride;
                                                            errs() << "\t"<< cmp->getName().str()<<" is true, but loop repeat; bounds[level].second = "<<bounds[level].second << "\n";
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            { /// I want to get the Begin of affine idx, But can't confirm is this right...?
                                /// thought only two operands in PHINode
                                indexbegin = Phi->getIncomingValueForBlock(PhiNext);
                                if (auto V = dyn_cast<ConstantInt>(indexbegin))
                                {
                                    if (ConstantInt *CI = dyn_cast<ConstantInt>(V))
                                    {
                                        bounds[level].first = CI->getSExtValue();
                                    }
                                    else if (ConstantFP *FP = dyn_cast<ConstantFP>(V))
                                    {
                                        bounds[level].first = (int)FP->getValueAPF().convertToFloat();
                                    }
                                }
                                else
                                {
                                    errs() << "\t" << Phi->getName().str() << " had non certain begin index\n";
                                    this->_nestLoopisAffine = false;
                                }
                            }
                        }
                    }
                }
            }*/
        } 
        if (auto LoopBound = nestloops[level]->getBounds(*SE)){
            errs() << "\tdirection: " << int(LoopBound.getPointer()->getDirection()) <<"\n";
            LoopBound.getPointer()->getFinalIVValue().dump();
            LoopBound.getPointer()->getInitialIVValue().dump();
            LoopBound.getPointer()->getStepInst().dump();
        }
        detected.insert(temNestBBs.begin(),temNestBBs.end());
    }
    _LoopexclusiveBBs = LoopexclusiveBBs;
    for(int level = 0; level < LoopexclusiveBBs.size(); level++){
        errs() << "level"<<level <<" has: ";
        for(auto elem:LoopexclusiveBBs[level]){
            errs() << elem->getName() <<", ";
        }
        errs() << ";\n";
    }
    // load stride information when loopsAffine is true
    // if loop is not affine--analyze END
    if(isloopsAffine()){
        setLoopsAffineStrides(strides);
    }else{
        addLoopExitNodes();
        return;
    }
    
    //print loop index information
    for(int i = 0; i < getLoopsAffineStrides().size(); i++){
        int stride = strides[i];
        varType left = bounds[i].first;
        varType right = bounds[i].second;
        varType count = 0;
        count = ((right - left + stride - 1)/stride);
        bounds[i].second = (count-1) * stride + left;
        // if(abs(stride)==1 || (right - left)%stride == 0)
        //     bounds[i].second -= stride;
        // else
        //     count+=1;
        /*
        if(bounds[i].first > bounds[i].second){
            int tem = bounds[i].first;
            bounds[i].first = bounds[i].second;
            bounds[i].second = tem;
        }
        */
        errs() << "loop level " << i << " stride " << getLoopsAffineStrides()[i] << " per cycle\n";
        errs() << "\tleftbound is " << bounds[i].first.to_string() << "\n";
        errs() << "\trightbound is " << bounds[i].second.to_string() << "\n";
        setLoopsAffineBounds(i,bounds[i].first, bounds[i].second);
        setLoopsAffineCounts(i,count);
    }
}

///recursively find array assess stride & bound
/*
std::vector<PHINode*> LLVMCDFG::arrayStride(Value* opnode, std::map<int, std::pair<double,std::pair<double,double>>>* factortable){
    
    //if return {} && factortable->clear() means non-affine operation
    if(factortable == NULL || factortable->empty()){
        if(dyn_cast<PHINode>(opnode)||dyn_cast<Constant>(opnode)){
            return {};
        }else if(auto ins = dyn_cast<Instruction>(opnode)){
            LLVMCDFGNode* temnode = node(ins);
            checkInsInputs(ins);
            for(int i = 0; i < ins->getNumOperands(); i++){
                auto preopnode = ins->getOperand(i);
                arrayStride(preopnode, NULL);
            }
        }
        if(factortable != NULL){factortable->clear();}
        return {};
    }
    if(dyn_cast<Constant>(opnode)){
        ///mul constant will be covered in following
        errs() << "\t\thas a constant address\n";
        return {};
    }else if(auto phi = dyn_cast<PHINode>(opnode)){
        std::vector<llvm::PHINode *> singlephi;
        ///TODO: can have conditional load/store?
        if(!PHItoLevel.count(phi)){
            factortable->clear();
            return {};
        }
        int level = PHItoLevel[phi];
        singlephi.push_back(phi);
        if((*factortable)[level].first == 0) (*factortable)[level].first = getLoopsAffineStrides(level);
        return singlephi;
    }else if(dyn_cast<StoreInst>(opnode)||dyn_cast<LoadInst>(opnode)){
        errs() << "\t\thas a read address\n";
        factortable->clear();
        return {};
    }else{
        Instruction* temins = dyn_cast<Instruction>(opnode);
        //errs()<<"\tcome accross ins:";temins->dump();
        int temopcode = temins->getOpcode();
        if(temins->isBinaryOp()){
            std::pair<int, int> OperandIndextoConst = BinaryConstOp(temins);
            //checkInsInputs(temins);//flatened DFG need no checkInsInputs
            switch (temopcode)
            {
            case Instruction::Mul :{
                if(OperandIndextoConst.second == 0){
                    factortable->clear();
                    arrayStride(temins->getOperand(0), NULL);
                    arrayStride(temins->getOperand(1), NULL);
                    return {};
                }else{
                    auto innerphis = arrayStride(temins->getOperand(1-OperandIndextoConst.first), factortable);
                    for(auto phi : innerphis){
                        int level = PHItoLevel[phi];
                        if((*factortable)[level].first == 0) (*factortable)[level].first = getLoopsAffineStrides(level);
                        (*factortable)[level].first *= OperandIndextoConst.second;
                        (*factortable)[level].second.first *= OperandIndextoConst.second;
                        (*factortable)[level].second.second *= OperandIndextoConst.second;
                    }
                    return innerphis;
                }
            }
            case Instruction::UDiv:
            case Instruction::SDiv:
            case Instruction::FDiv:{
                if(OperandIndextoConst.second == 0){
                    factortable->clear();
                    arrayStride(temins->getOperand(0), NULL);
                    arrayStride(temins->getOperand(1), NULL);
                    return {};
                }if(OperandIndextoConst.first == 0){
                    factortable->clear();
                    arrayStride(temins->getOperand(0), NULL);
                    arrayStride(temins->getOperand(1), NULL);
                    return {};
                }else{
                    auto innerphis = arrayStride(temins->getOperand(0), factortable);
                    for(auto phi : innerphis){
                        int level = PHItoLevel[phi];
                        if((*factortable)[level].first == 0) (*factortable)[level].first = getLoopsAffineStrides(level);
                        (*factortable)[level].first /= OperandIndextoConst.second;
                        (*factortable)[level].second.first /= OperandIndextoConst.second;
                        (*factortable)[level].second.second /= OperandIndextoConst.second;                        
                    }
                    return innerphis;
                }
            }
            ///TODO:div & Lshl may be unaffine when not divisible
            case Instruction::Shl :{
                if(OperandIndextoConst.second == 0){
                    factortable->clear();
                    arrayStride(temins->getOperand(0), NULL);
                    arrayStride(temins->getOperand(1), NULL);
                    return {};
                }if(OperandIndextoConst.first == 0){
                    factortable->clear();
                    arrayStride(temins->getOperand(0), NULL);
                    arrayStride(temins->getOperand(1), NULL);
                    return {};
                }else{
                    auto innerphis = arrayStride(temins->getOperand(0), factortable);
                    for(auto phi : innerphis){
                        int level = PHItoLevel[phi];
                        if((*factortable)[level].first == 0) (*factortable)[level].first = getLoopsAffineStrides(level);
                        //errs()<< "shl: " << (int)(*factortable)[level].first<<"<<"<<OperandIndextoConst.second<<"!!!!!!\n";
                        (*factortable)[level].first = (int)(*factortable)[level].first << OperandIndextoConst.second;
                        //errs()<<"result is: "<< (int)(*factortable)[level].first<<"!!!!\n";
                        (*factortable)[level].second.first = (int)(*factortable)[level].second.first << OperandIndextoConst.second;
                        (*factortable)[level].second.second = (int)(*factortable)[level].second.second << OperandIndextoConst.second;
                    }
                    return innerphis;
                }
            }
            case Instruction::LShr :{
                if(OperandIndextoConst.second == 0){
                    factortable->clear();
                    arrayStride(temins->getOperand(0), NULL);
                    arrayStride(temins->getOperand(1), NULL);
                    return {};
                }if(OperandIndextoConst.first == 0){
                    factortable->clear();
                    arrayStride(temins->getOperand(0), NULL);
                    arrayStride(temins->getOperand(1), NULL);
                    return {};
                }else{
                    auto innerphis = arrayStride(temins->getOperand(0), factortable);
                    for(auto phi : innerphis){
                        int level = PHItoLevel[phi];
                        if((*factortable)[level].first == 0) (*factortable)[level].first = getLoopsAffineStrides(level);
                        (*factortable)[level].first /= pow(2, OperandIndextoConst.second);
                        (*factortable)[level].second.first /= pow(2, OperandIndextoConst.second);
                        (*factortable)[level].second.second /= pow(2, OperandIndextoConst.second);
                    }
                    return innerphis;
                }
            }
            case Instruction::Add :{
                if(OperandIndextoConst.second == 0){
                    auto temphis0 = arrayStride(temins->getOperand(0), factortable);
                    auto temphis1 = arrayStride(temins->getOperand(1), factortable);
                    temphis1.insert(temphis1.end(), temphis0.begin(), temphis0.end());///TODO:10*(i+i) ???
                    return temphis1;
                }else{
                    auto innerphis = arrayStride(temins->getOperand(1-OperandIndextoConst.first), factortable);
                    if(innerphis.empty()){//may add something else
                        factortable->clear();
                        return {};
                    }
                    auto phi = *innerphis.begin();//only add once
                    int level = PHItoLevel[phi];
                    (*factortable)[level].second.first += OperandIndextoConst.second;
                    (*factortable)[level].second.second += OperandIndextoConst.second;
                    
                    return innerphis;
                }
            }
            case Instruction::Or :{
                if(OperandIndextoConst.second == 0){//unkonw condition
                    //assert(0&&"unknow condition about Or operation");
                    factortable->clear();
                    arrayStride(temins->getOperand(0), NULL);
                    arrayStride(temins->getOperand(1), NULL);
                    return {};
                }else{
                    auto innerphis = arrayStride(temins->getOperand(1-OperandIndextoConst.first), factortable);
                    for(auto phi : innerphis){//TODO:any else condition besides add?
                        int level = PHItoLevel[phi];
                        (*factortable)[level].second.first += OperandIndextoConst.second;
                        (*factortable)[level].second.second += OperandIndextoConst.second;
                    }
                    return innerphis;
                }
            }
            case Instruction::Sub :{
                if(OperandIndextoConst.second == 0){
                    auto temphis0 = arrayStride(temins->getOperand(0), factortable);
                    auto temphis1 = arrayStride(temins->getOperand(1), factortable);
                    for(auto phi : temphis1){///to Negate subtrahends
                        int level = PHItoLevel[phi];
                        if((*factortable)[level].first == 0) (*factortable)[level].first = 1;
                        (*factortable)[level].first = -(*factortable)[level].first;
                    }
                    temphis1.insert(temphis1.end(), temphis0.begin(), temphis0.end());
                    return temphis1;
                }else if(OperandIndextoConst.first == 0){
                    auto temphis1 = arrayStride(temins->getOperand(1), factortable);
                    auto phi = *temphis1.begin();///to Negate subtrahends
                                                ///constant only once
                        int level = PHItoLevel[phi];
                        if((*factortable)[level].first == 0) (*factortable)[level].first = 1;
                        (*factortable)[level].first = -(*factortable)[level].first;
                        (*factortable)[level].second.first = OperandIndextoConst.second - (*factortable)[level].second.first;
                        (*factortable)[level].second.second =  OperandIndextoConst.second - (*factortable)[level].second.second;
                }else{
                    auto innerphis = arrayStride(temins->getOperand(0), factortable);
                    auto phi = *innerphis.begin();///constant only once
                        int level = PHItoLevel[phi];
                        (*factortable)[level].second.first -= OperandIndextoConst.second;
                        (*factortable)[level].second.second -= OperandIndextoConst.second;
                    return innerphis;
                }
            }
            default:{
                factortable->clear();
                return {};
                }
            }
        }else{///when will is not BinaryOp??!
            if(auto temins = dyn_cast<ZExtInst>(opnode)){
                errs()<<"finally across ZExt";temins->dump();
                auto innerphis = arrayStride(temins->getOperand(0), factortable);
                return innerphis;
            }else{
                assert(0&&"what ins?");
            }
        }
    }
}
*/

///recursively find array assess stride & bound
std::vector<PHINode*> LLVMCDFG::arrayStride(LLVMCDFGNode* opnode, std::map<int, std::pair<double,std::pair<varType, varType>>>* factortable){
    auto ins = opnode->instruction();
    //if return {} && factortable->clear() means non-affine operation
    if(factortable == NULL || factortable->empty()){
        if(factortable != NULL){factortable->clear();}
        return {};
    }
    if(opnode->hasConst()){
        ///mul constant will be covered in following
        errs() << "\t\thas a constant address\n";
        return {};
    }else if(ins != NULL && (dyn_cast<PHINode>(ins))){
        auto phi = dyn_cast<PHINode>(ins);
        std::vector<llvm::PHINode *> singlephi;
        ///TODO: can have conditional load/store?
        if(!PHItoLevel.count(phi)){
            factortable->clear();
            return {};
        }
        int level = PHItoLevel[phi];
        singlephi.push_back(phi);
        if((*factortable)[level].first == 0) (*factortable)[level].first = getLoopsAffineStrides(level);
        return singlephi;
    }else if(ins != NULL && (dyn_cast<StoreInst>(ins)||dyn_cast<LoadInst>(ins))){
        errs() << "\t\thas a read address\n";
        factortable->clear();
        return {};
    }else{
        int temopcode;
        if(ins != NULL){
            temopcode = ins->getOpcode();
        }else{
            if(opnode->customInstruction() == "ADD"){
                temopcode = Instruction::Add;
            }else if(opnode->customInstruction() == "MUL"){
                temopcode = Instruction::Mul;
            }
        }
        
        if(opnode->inputNodes().size() == 2){
            std::pair<int, int> OperandIndextoConst = BinaryConstOp(opnode);
            //checkInsInputs(temins);//flatened DFG need no checkInsInputs
            switch (temopcode)
            {
            case Instruction::Mul :{
                if(OperandIndextoConst.first == -1){
                    factortable->clear();
                    arrayStride(opnode->getInputPort(0), NULL);
                    arrayStride(opnode->getInputPort(1), NULL);
                    return {};
                }else{
                    int coefficient = OperandIndextoConst.second;
                    auto innerphis = arrayStride(opnode->getInputPort(1-OperandIndextoConst.first), factortable);
                    for(auto phi : innerphis){
                        int level = PHItoLevel[phi];
                        if((*factortable)[level].first == 0) (*factortable)[level].first = getLoopsAffineStrides(level);
                        (*factortable)[level].first *= coefficient;
                        (*factortable)[level].second.first *= coefficient;
                        (*factortable)[level].second.second *= coefficient;
                    }
                    return innerphis;
                }
            }
            case Instruction::UDiv:
            case Instruction::SDiv:
            case Instruction::FDiv:{
                if(OperandIndextoConst.first != 1){
                    factortable->clear();
                    arrayStride(opnode->getInputPort(0), NULL);
                    arrayStride(opnode->getInputPort(1), NULL);
                    return {};
                }else{
                    int coefficient = OperandIndextoConst.second;
                    auto innerphis = arrayStride(opnode->getInputPort(0), factortable);
                    for(auto phi : innerphis){
                        int level = PHItoLevel[phi];
                        if((*factortable)[level].first == 0) (*factortable)[level].first = getLoopsAffineStrides(level);
                        (*factortable)[level].first /= coefficient;
                        (*factortable)[level].second.first /= coefficient;
                        (*factortable)[level].second.second /= coefficient;                        
                    }
                    return innerphis;
                }
            }
            ///TODO:div & Lshl may be unaffine when not divisible
            case Instruction::Shl :{
                if(OperandIndextoConst.first != 1){
                    factortable->clear();
                    arrayStride(opnode->getInputPort(0), NULL);
                    arrayStride(opnode->getInputPort(1), NULL);
                    return {};
                }else{
                    int coefficient = OperandIndextoConst.second;
                    auto innerphis = arrayStride(opnode->getInputPort(0), factortable);
                    for(auto phi : innerphis){
                        int level = PHItoLevel[phi];
                        if((*factortable)[level].first == 0) (*factortable)[level].first = getLoopsAffineStrides(level);
                        //errs()<< "shl: " << (int)(*factortable)[level].first<<"<<"<<Coefficient<<"!!!!!!\n";
                        (*factortable)[level].first = (int)(*factortable)[level].first << coefficient;
                        //errs()<<"result is: "<< (int)(*factortable)[level].first<<"!!!!\n";
                        (*factortable)[level].second.first = (*factortable)[level].second.first * pow(2, coefficient);
                        (*factortable)[level].second.second = (*factortable)[level].second.second * pow(2, coefficient);
                    }
                    return innerphis;
                }
            }
            case Instruction::LShr :{
                if(OperandIndextoConst.first != 1){
                    factortable->clear();
                    arrayStride(opnode->getInputPort(0), NULL);
                    arrayStride(opnode->getInputPort(1), NULL);
                    return {};
                }else{
                    int coefficient = OperandIndextoConst.second;
                    auto innerphis = arrayStride(opnode->getInputPort(0), factortable);
                    for(auto phi : innerphis){
                        int level = PHItoLevel[phi];
                        if((*factortable)[level].first == 0) (*factortable)[level].first = getLoopsAffineStrides(level);
                        (*factortable)[level].first /= pow(2, OperandIndextoConst.second);
                        (*factortable)[level].second.first /= pow(2, OperandIndextoConst.second);
                        (*factortable)[level].second.second /= pow(2, OperandIndextoConst.second);
                    }
                    return innerphis;
                }
            }
            case Instruction::Add :{
                if(OperandIndextoConst.first == -1){
                    auto temphis0 = arrayStride(opnode->getInputPort(0), factortable);
                    auto temphis1 = arrayStride(opnode->getInputPort(1), factortable);
                    temphis1.insert(temphis1.end(), temphis0.begin(), temphis0.end());///TODO:10*(i+i) ???
                    if(!factortable->empty())
                        return temphis1;
                    else
                        return {};
                }else{
                    int coefficient = OperandIndextoConst.second;
                    auto innerphis = arrayStride(opnode->getInputPort(1-OperandIndextoConst.first), factortable);
                    if(innerphis.empty()){//may add something else
                        factortable->clear();
                        return {};
                    }
                    auto phi = *innerphis.begin();//only add once
                    int level = PHItoLevel[phi];
                    (*factortable)[level].second.first += coefficient;
                    (*factortable)[level].second.second += coefficient;
                    
                    return innerphis;
                }
            }
            case Instruction::Or :{
                if(OperandIndextoConst.first == -1){//unkonw condition
                    //assert(0&&"unknow condition about Or operation");
                    factortable->clear();
                    arrayStride(opnode->getInputPort(0), NULL);
                    arrayStride(opnode->getInputPort(1), NULL);
                    return {};
                }else{
                    int coefficient = OperandIndextoConst.second;
                    auto loopbound = _loopsAffineBounds;
                    auto loopstride = _loopsAffineStrides;
                    auto innerphis = arrayStride(opnode->getInputPort(1-OperandIndextoConst.first), factortable);
                    for(auto phi : innerphis){//TODO:any else condition besides add?
                        int level = PHItoLevel[phi];
                        if(loopbound[level].first == 0 && loopstride[level] > coefficient){
                            (*factortable)[level].second.first += coefficient;
                            (*factortable)[level].second.second += coefficient;
                        }else{
                            factortable->clear();
                            arrayStride(opnode->getInputPort(0), NULL);
                            arrayStride(opnode->getInputPort(1), NULL);
                            return {};
                        }
                    }
                    return innerphis;
                }
            }
            case Instruction::Sub :{
                if(OperandIndextoConst.first == -1){
                    auto temphis0 = arrayStride(opnode->getInputPort(0), factortable);
                    auto temphis1 = arrayStride(opnode->getInputPort(1), factortable);
                    if(!factortable->empty()){
                        for(auto phi : temphis1){///to Negate subtrahends
                            int level = PHItoLevel[phi];
                            if((*factortable)[level].first == 0) (*factortable)[level].first = 1;
                            (*factortable)[level].first = -(*factortable)[level].first;
                        }
                        temphis1.insert(temphis1.end(), temphis0.begin(), temphis0.end());
                        return temphis1;
                    }else{
                        return {};
                    }
                }else if(OperandIndextoConst.first == 0){
                    int coefficient = OperandIndextoConst.second;
                    auto temphis1 = arrayStride(opnode->getInputPort(1), factortable);
                    auto phi = *temphis1.begin();///to Negate subtrahends
                                                ///constant only once
                        int level = PHItoLevel[phi];
                        //stride
                        if((*factortable)[level].first == 0){
                            (*factortable)[level].first = 1;
                        }
                        (*factortable)[level].first = -(*factortable)[level].first;
                        //bound
                        (*factortable)[level].second.first = coefficient - (*factortable)[level].second.first;
                        (*factortable)[level].second.second =  coefficient - (*factortable)[level].second.second;
                }else{
                    int coefficient = OperandIndextoConst.second;
                    auto innerphis = arrayStride(opnode->getInputPort(0), factortable);
                    auto phi = *innerphis.begin();///constant only once
                        int level = PHItoLevel[phi];
                        (*factortable)[level].second.first -= coefficient;
                        (*factortable)[level].second.second -= coefficient;
                    return innerphis;
                }
            }
            default:{
                factortable->clear();
                return {};
                }
            }
        }else{///when will is not BinaryOp??!
            if(auto temins = dyn_cast<ZExtInst>(ins)){
                errs()<<"finally across ZExt";temins->dump();
                auto innerphis = arrayStride(opnode->getInputPort(0), factortable);
                return innerphis;
            }else{
                return {};
                assert(0&&"what ins?");
            }
        }
    }
}

/*
///to get the Branch of this loop 
BranchInst *LooptoBranch(std::set<llvm::BasicBlock *> detected, std::set<llvm::BasicBlock *> temNestBBs){
    for(auto BI = temNestBBs.begin(); BI != temNestBBs.end(); BI++){
        BasicBlock *temBB = *BI;
        if(detected.find(temBB) == detected.end()){
            for(auto II = temBB->begin(); II != temBB->end(); II++){
                Instruction *ins = &*II;
                if(auto br = dyn_cast<BranchInst>(ins)){
                    if(br->getNumOperands() == 3){
                        errs() << "yes: "<< br->getOperand(0)->getName().str() <<"\n";
                        return br;
                    }
                }
            }
        }
    }
    return NULL;
}
*/

//nestloop affine Analyze
void LLVMCDFG::affineAnalyze(){

    if(!isloopsAffine()) return;

	outs() << ">>>>>> analyze Load/Store Ins with affine access\n";
    //find LSU & analyze their access pattern
    for(auto &elem : _nodes){
        auto node = elem.second;
        Instruction *ins = node->instruction();
        GetElementPtrInst *GEP = NULL;
        varType startoffset = 0;
        bool isGEPaffine = true;
        std::map<int, int> LSstride;
        varType LSbounds[3] = {0, 0, 0};
        int addressidx;
        //initial LSstride for each nest loop
        for(int i = 0; i < getLoopsAffineStrides().size(); i++){
            LSstride[i] = 0;
        }
        if(ins == NULL){
            continue;
        }
        if(auto Loadins = dyn_cast<LoadInst>(ins)){
            GEP = dyn_cast<GetElementPtrInst>(Loadins->getPointerOperand());
            addressidx = 0;
        }else if(auto Storeins = dyn_cast<StoreInst>(ins)){
            GEP = dyn_cast<GetElementPtrInst>(Storeins->getPointerOperand());
            addressidx = 1;
        }else{
            continue;
        }
        errs()<<"--------------come accross LSU: "<<node->getName()<<" instruction: ";ins->dump();
        if(GEP == NULL){continue;}
        ///array element dimension's scale
        bool allzerostride = true;
        errs() << "handle LSPattern:\n";
        int elemsize = DL->getTypeAllocSize(GEP->getResultElementType());
        LSbounds[2] = elemsize;
        errs() << "LS size is " << elemsize <<" \n";
        /*
            here factortable's key is nestloop level; value is for this levelindex
            temporary dimension's stride & bounds: first element represent stride; second element represent boundspair
            for boundspair: first element represent leftbounds; second element represent rightbounds
        */
        std::map<int, std::pair<double, std::pair<varType,varType>>> factortable;
        ///factortable initialize is must!!
        for(int i = 0; i < getLoopsAffineStrides().size(); i++){
            factortable[i].first = 0;
            factortable[i].second.first = getLoopsAffineBounds(i).first;
            factortable[i].second.second = getLoopsAffineBounds(i).second;
        }

        
        arrayStride(node->getInputPort(addressidx), &factortable);

        if(factortable.empty()){
            errs() << "\t\t[this GEP is not affine]\n";
            isGEPaffine = false;
        }
        else
        {
            for (int i = 0; i < factortable.size(); i++)
            {
                varType left, right;
                int stride = (int)factortable[i].first;
                left = factortable[i].second.first;
                right = factortable[i].second.second;
                ///TODO: maybe need to convert accoring to stride's +- ?
                if (left.index()!=2 && right.index()!=2)
                {
                    if(std::get<int>(left.value) > std::get<int>(right.value)){
                        right = factortable[i].second.first;
                        left = factortable[i].second.second;
                    }
                }
                LSstride[i] += (stride);
                if (stride)
                { // if stride, this index is calculated in this dimension
                    allzerostride = false;
                    startoffset += (factortable[i].second.first);
                    LSbounds[0] += left;
                    LSbounds[1] += right;
                }
                errs() << "\t\tin this GEP " << i << " stride: " << (int)factortable[i].first << " ; leftbound: " << factortable[i].second.first.to_string() << " ; rightbound: " << factortable[i].second.second.to_string() << "\n";
            }
        }
        varType varLSoffset = node->getLSoffset();
        startoffset += varLSoffset;
        if(allzerostride)
            startoffset = "";///assign for totally constant GEP
        if(isGEPaffine){
            node->setLSstride(LSstride);
            node->setLSbounds(LSbounds);
            node->setLSstart(startoffset);
        }
        else{
            Type *currType = GEP->getSourceElementType();
            auto currTypesize = DL->getTypeAllocSize(currType);
            LSbounds[0] = 0;
            LSbounds[1] = (int)currTypesize-elemsize;
            LSbounds[2] = elemsize;
            node->setLSbounds(LSbounds);
        }
    }

}


// remove GEP nodes(when pattern)
void LLVMCDFG::handleAffineLSNodes(){
    bool allaffine = true;
    std::set<LLVMCDFGNode*> GEPsToDelnode, GEPsToDeltree;
    std::map<int, LLVMCDFGNode *> nodes = _nodes;
    for(auto &elem : nodes){
        auto node = elem.second;
        Instruction *ins = node->instruction();
        std::string customIns = node->customInstruction();
        if(node->isLSaffine()){
            // get pattern access offset
            auto LSbounds = node->getLSbounds();
            varType offset = LSbounds[0] + node->getLSoffset();
            errs() <<"LSbounds[0]: " << LSbounds[0].to_string() << "\n";
            errs() <<"offset = LSbounds[0] + node->getLSoffset(): " << offset.to_string() << "\n";
            node->setLSoffset(offset);
            // delete address claculation
            errs() << node->getName() << " is affine, delete its GEP tree\n";
            auto teminputs = node->inputNodes();
            for(auto pre : teminputs){
                if(dyn_cast<LoadInst>(ins)){
                    GEPsToDeltree.insert(node->getInputPort(0));
                }else if(dyn_cast<StoreInst>(ins)){
                    GEPsToDeltree.insert(node->getInputPort(1));
                }
            }
        }else if(ins != NULL){
            if(dyn_cast<LoadInst>(ins) || dyn_cast<StoreInst>(ins)){
                allaffine = false;
                auto temp =node->inputNodes();
                for(auto pre : temp){
                    auto preins = pre->instruction();
                    if(preins != NULL){
                        if(dyn_cast<GetElementPtrInst>(preins)){
                            errs() << node->getName() << " is not affine, delete its GEP node\n";
                            //delete GEP & connect lastmul of address claculation with Load/Store
                            if(pre->inputNodes().size() == 0)
                                continue;
                            auto lastmul = *(pre->inputNodes().begin());
                            lastmul->addOutputNode(node);
                            node->addInputNode(lastmul, node->getInputIdx(pre));
                            addEdge(lastmul, node, EDGE_TYPE_DATA);
                            GEPsToDelnode.insert(pre);
                        }
                    }
                }
            } 
        }
    }
    ///finally del to avoid muiti LSU using the same GEP
    for(auto del:GEPsToDelnode){
        delNode(del);
    }
    for(auto deltree:GEPsToDeltree){
        delNodeTree(deltree);
    }

}

//to judge if a node is inline ADD or Custom ADD
bool isADDNode(LLVMCDFGNode* node){
    auto ins = node->instruction();
    bool isADD = false;
    if(ins == NULL){
        if(node->customInstruction() == "ADD"){
            isADD = true;
        }
    }
    else if(ins->getOpcode() == Instruction::Add || ins->getOpcode() == Instruction::FAdd){
        isADD = true;
    }
    return isADD;
}

void LLVMCDFG::transAddOpSelectTree(){
    outs() << ">>>>>>>>>>transform add result SELECT tree to add operand SELECT tree";
    std::map<int, LLVMCDFGNode *> nodes = _nodes;
    for (auto &elem : nodes){
        auto node = elem.second;
        Instruction* ins = node->instruction();
        if(node->customInstruction() == "SELECT"){
            outs() << "come across node: "<< node->getName() <<":\n";
            std::map<LLVMCDFGNode*, LLVMCDFGNode*> selectTree;
            std::vector<LLVMCDFGNode*> op;
            op.push_back(node->getInputPort(0));
            op.push_back(node->getInputPort(1));
            int Addidx = -1;
            bool firstADD = isADDNode(op[0]);
            bool secondADD = isADDNode(op[1]);
            bool bothAdd =  secondADD && firstADD;
            LLVMCDFGNode *sameOperand = NULL;
            if (bothAdd)
            {
                auto op0inputs = op[0]->inputNodes();
                if (std::count(op0inputs.begin(), op0inputs.end(), op[1]->getInputPort(0)))
                {
                    sameOperand = op[1]->getInputPort(0);
                }
                else if (std::count(op0inputs.begin(), op0inputs.end(), op[1]->getInputPort(1)))
                {
                    sameOperand = op[1]->getInputPort(1);
                }
            }
            else
            {
                if (firstADD)
                {
                    Addidx = 0;
                }
                else if (secondADD)
                {
                    Addidx = 1;
                }
                if (Addidx == -1)
                {
                    continue;
                }
                auto Addinputs = op[Addidx]->inputNodes();
                if (std::count(Addinputs.begin(), Addinputs.end(), op[1 - Addidx]))
                {
                    sameOperand = op[1 - Addidx];
                    auto NewADD = addNode("ADD", sameOperand->BB());
                    auto NewConst0 = addNode("CONST", sameOperand->BB());
                    NewConst0->setConstVal(0);
                    sameOperand->addOutputNode(NewADD);
                    NewConst0->addOutputNode(NewADD);
                    NewADD->addOutputNode(node);
                    NewADD->addInputNode(sameOperand, 0);
                    NewADD->addInputNode(NewConst0, 1);
                    node->addInputNode(NewADD, 1 - Addidx);
                    delEdge(edge(sameOperand, node));
                    addEdge(NewConst0, NewADD, EDGE_TYPE_DATA);
                    addEdge(sameOperand, NewADD, EDGE_TYPE_DATA);
                    addEdge(NewADD, node, EDGE_TYPE_DATA);
                    op[1 - Addidx] = NewADD;
                }
            }
            if (sameOperand == NULL)
            {
                // do not have the same add operand input
                continue;
            }
            selectTree[node] = node; // mark this SELECT node by this
            std::map<LLVMCDFGNode *, std::set<LLVMCDFGNode *>> treeNodes2Outs;
            std::map<LLVMCDFGNode *, LLVMCDFGNode *> selOut2selTreeNode;
            std::vector<LLVMCDFGNode *> SelsStack;
            SelsStack.insert(SelsStack.begin(), node);
            while (!SelsStack.empty())
            {
                LLVMCDFGNode *temSel = SelsStack.back();
                SelsStack.pop_back();
                auto temOutputs = temSel->outputNodes();
                for (auto outputNode : temOutputs)
                {
                    int idx = outputNode->getInputIdx(temSel);
                    if (outputNode->customInstruction() == "SELECT")
                    {
                        LLVMCDFGNode *anotherSelOp = outputNode->getInputPort(1 - idx);
                        Instruction *anotherSelOpIns = anotherSelOp->instruction();
                        if (anotherSelOp->customInstruction() == "SELECT" && selectTree.count(anotherSelOp))
                        {
                            SelsStack.insert(SelsStack.begin(), outputNode);
                            selectTree[outputNode] = NULL;
                            treeNodes2Outs[selOut2selTreeNode[outputNode]].erase(outputNode);
                            continue;
                        }
                        else if (isADDNode(anotherSelOp))
                        {
                            if (anotherSelOp->getInputIdx(sameOperand) != -1)
                            {
                                selectTree[outputNode] = anotherSelOp;
                                SelsStack.insert(SelsStack.begin(), outputNode);
                                continue;
                            }
                        }
                        else
                        {
                            selOut2selTreeNode[outputNode] = temSel;
                        }
                    }
                    treeNodes2Outs[temSel].insert(outputNode);
                }
            }
            /// transform the tree
            for (auto &treeNodeElem : selectTree)
            {
                auto treeNode = treeNodeElem.first;
                auto ADDNode = treeNodeElem.second;
                bool firstNode = false;
                if (ADDNode == NULL)
                {
                    continue;
                }
                if (ADDNode == treeNode)
                {
                    firstNode = true;
                    ADDNode = op[0];
                }
                int idx = treeNode->getInputIdx(ADDNode);
                auto addOpNode = ADDNode->getInputPort(1 - ADDNode->getInputIdx(sameOperand));
                bool isbackedge = ADDNode->isOutputBackEdge(treeNode);
                if (ADDNode->outputNodes().size() == 1)
                {
                    delNode(ADDNode);
                    treeNode->addInputNode(addOpNode, idx, isbackedge);
                    addOpNode->addOutputNode(treeNode, isbackedge);
                    addEdge(addOpNode, treeNode, EDGE_TYPE_DATA);
                }
                else
                {
                    delEdge(edge(ADDNode, treeNode));
                    treeNode->addInputNode(addOpNode, idx, isbackedge);
                    addOpNode->addOutputNode(treeNode, isbackedge);
                    addEdge(addOpNode, treeNode, EDGE_TYPE_DATA);
                }
                if (firstNode)
                {
                    ADDNode = op[1];
                    isbackedge = ADDNode->isOutputBackEdge(treeNode);
                    int idx = treeNode->getInputIdx(ADDNode);
                    auto addOpNode = ADDNode->getInputPort(1 - ADDNode->getInputIdx(sameOperand));
                    if (ADDNode->outputNodes().size() == 1)
                    {
                        delNode(ADDNode);
                        treeNode->addInputNode(addOpNode, idx, isbackedge);
                        addOpNode->addOutputNode(treeNode, isbackedge);
                        addEdge(addOpNode, treeNode, EDGE_TYPE_DATA);
                    }
                    else
                    {
                        delEdge(edge(ADDNode, treeNode));
                        treeNode->addInputNode(addOpNode, idx, isbackedge);
                        addOpNode->addOutputNode(treeNode, isbackedge);
                        addEdge(addOpNode, treeNode, EDGE_TYPE_DATA);
                    }
                }
                /// add ADD node for out of this tree
                if (treeNodes2Outs.count(treeNode) && treeNodes2Outs[treeNode].size() > 0)
                {
                    auto NewADD = addNode("ADD", treeNode->BB());
                    NewADD->addInputNode(treeNode, 1);
                    NewADD->addInputNode(sameOperand, 0);
                    treeNode->addOutputNode(NewADD);
                    sameOperand->addOutputNode(NewADD);
                    addEdge(treeNode, NewADD, EDGE_TYPE_DATA);
                    addEdge(sameOperand, NewADD, EDGE_TYPE_DATA);
                    for (auto extraOut : treeNodes2Outs[treeNode])
                    {
                        bool isbackedge = treeNode->isOutputBackEdge(extraOut);
                        int idx = extraOut->getInputIdx(treeNode);
                        delEdge(edge(treeNode, extraOut));
                        extraOut->addInputNode(NewADD, idx, isbackedge);
                        NewADD->addOutputNode(extraOut, isbackedge);
                        addEdge(NewADD, extraOut, EDGE_TYPE_DATA);
                    }
                }
            }
        }
    }
}

// analyze SELECT tree from backedge node
bool analyzeSelectTree(LLVMCDFGNode* node, LLVMCDFGNode* rootnode, std::set<LLVMCDFGNode*> &selectTree){
    errs() << "temnode is: " << node->getName()<<", root node: " << rootnode->getName() << "\n";
    bool isReachable = false;
    for(auto inputNode: node->inputNodes()){
        if(node->isInputBackEdge(inputNode)){
            continue;
        }
        if(inputNode == rootnode){
            isReachable = true;
        }else if(analyzeSelectTree(inputNode, rootnode, selectTree)){
            isReachable = true;
        }
    }
    if(isReachable){
        errs() << "\tthis node is reachable!\n";
        selectTree.insert(node);
    }
    return isReachable;
}

///traverse all initialSels & generate ACC node
void LLVMCDFG::generateACC(std::set<LLVMCDFGNode *> &InitialSels){
    std::set<LLVMCDFGNode*> rmnodes;
    for (auto node : InitialSels)
    {
        LLVMCDFGNode *initSel = node;
        auto Initial = node->getInputPort(0);
        auto pre = node->getInputPort(1);
        errs() << "\n\tcurrent pre is: " << pre->getName() << "\n";
        if (pre->isOutputBackEdge(node))
        {
            if (isADDNode(pre))
            {
                auto temoutputedges = node->outputEdges();
                LLVMCDFGNode *cycleout = NULL;
                LLVMCDFGNode *ADDpre = NULL;
                bool findcycle = false;
                bool legalexchange = true;
                // to check add chain and find cycleout(if don't have cycleout, no ACC)
                for (auto addpre : pre->inputNodes())
                {
                    legalexchange = true;
                    errs() << "tem ADD pre: " << addpre->getName() << "\n";
                    std::vector<LLVMCDFGNode *> todetect;
                    todetect.push_back(addpre);
                    while (!findcycle && !todetect.empty())
                    {
                        auto temnode = todetect.back();
                        todetect.pop_back();
                        auto temins = temnode->instruction();
                        if (temnode != initSel)
                        {
                            if (!isADDNode(temnode))
                            {
                                continue;
                            }
                        }
                        for (auto prenode : temnode->inputNodes())
                        {
                            if (prenode == initSel)
                            {
                                findcycle = true;
                                break;
                            }
                            todetect.push_back(prenode);
                        }
                        if (findcycle)
                        {
                            cycleout = temnode;
                            ADDpre = addpre;
                        }
                    }
                    if (findcycle)
                    {
                        errs() << "find cycle out: " << cycleout->getName() << "!!!!!!!!!!!!\n";
                        break;
                    }
                }
                // then handle related nodes to generate ACC
                if (cycleout != NULL)
                {
                    if (cycleout != pre && legalexchange)
                    { // exchange add oprands
                    ///TODO: if there is other outputs from the middle of addchain
                        errs() << "cycleout: " << cycleout->getName() << "; backedge pre: " << pre->getName() << "\n";
                        LLVMCDFGNode *transoperand;
                        int preidx, cycleoutidx;
                        for (auto preinput : pre->inputNodes())
                        {
                            errs() << "\ttem backedge pre's input is: " << preinput->getName() << "\n";
                            if (preinput == ADDpre)
                            {
                                errs() << "\t\tdetected: pass\n";
                                continue;
                            }
                            else
                            {
                                errs() << "\t\tnot in cycle: record as transoperand\n";
                                transoperand = preinput;
                                preidx = pre->getInputIdx(preinput);
                            }
                        }
                        cycleoutidx = cycleout->getInputIdx(node);
                        node->addOutputNode(pre);
                        pre->addInputNode(node, preidx);
                        addEdge(node, pre, EDGE_TYPE_DATA);

                        cycleout->addInputNode(transoperand, cycleoutidx);
                        transoperand->addOutputNode(cycleout);
                        addEdge(transoperand, cycleout, EDGE_TYPE_DATA);

                        node->delOutputNode(cycleout);
                        cycleout->delInputNode(node);
                        auto temedge = edge(node, cycleout);
                        delEdge(temedge);

                        transoperand->delOutputNode(pre);
                        pre->delInputNode(transoperand);
                        temedge = edge(transoperand, pre);
                        delEdge(temedge);
                    }
                    delEdge(edge(Initial, node));
                    for (auto findinc : pre->inputNodes())
                    {
                        if (findinc == node)
                            continue;
                        else
                        {
                            node->addInputNode(findinc, 0);
                            findinc->addOutputNode(node);
                            addEdge(findinc, node, EDGE_TYPE_DATA); /// TODO:here EDGE_TYPE_DATA?
                        }
                    }
                    errs() << "node->outputEdges(): " << node->outputEdges().size() << "**********\n";
                    if (node->outputEdges().size() == 1) 
                    {                                    // only output to addself
                        node->setAccFirst();
                        // pre->delInputNode(node);//delete node form add's IOmap
                        pre->delOutputNode(node);
                        for (auto addsucc : pre->outputNodes())
                        {
                            if (pre->isOutputBackEdge(addsucc))
                            { // TODO:is here right to skip ever backedge?
                                continue;
                            }
                            addsucc->addInputNode(node);
                            node->addOutputNode(addsucc);
                            addsucc->setInputIdx(node, addsucc->getInputIdx(pre));
                            addEdge(node, addsucc, EDGE_TYPE_DATA);
                        }
                        errs() << " SELECT has no other outputs---delete add calculate: " << pre->getName() << "\n";
                        node->delInputNode(pre);
                        rmnodes.insert(pre);
                    }
                    else if(pre->outputEdges().size() == 1)
                    {
                        errs() << "SELECT has succ: ";
                        for (auto succ : node->outputNodes())
                        {
                            errs() << succ->getName() << " ";
                        }
                        errs() << "\n";
                        errs() << "ADD has no other outputs---delete add calculate: " << pre->getName() << "\n";
                        node->delInputNode(pre);
                        rmnodes.insert(pre);
                    }else{
                        delEdge(edge(pre, node));
                        errs() << "ADD and SELECT both have other outputs\n";
                    }
                    //handle initial(const: set constVal; others: set 0 as initial, ADD original initial variable)
                    if(Initial->hasConst()){
                        node->setInitial(Initial->constVal());
                        rmnodes.insert(Initial);
                    }else{
                        node->setInitial(0);
                        auto InitialADD = addNode("ADD", Initial->BB());
                        auto ACCOutputs = node->outputNodes();
                        for(auto ACCOutput:ACCOutputs){
                            int idx = ACCOutput->getInputIdx(node);
                            delEdge(edge(node, ACCOutput));
                            addEdge(InitialADD, ACCOutput, EDGE_TYPE_DATA);
                            InitialADD->addOutputNode(ACCOutput);
                            ACCOutput->addInputNode(InitialADD, idx);
                        }
                        InitialADD->addInputNode(Initial, 0);
                        Initial->addOutputNode(InitialADD);
                        addEdge(Initial, InitialADD, EDGE_TYPE_DATA);
                        InitialADD->addInputNode(node, 1);
                        node->addOutputNode(InitialADD);
                        addEdge(node, InitialADD, EDGE_TYPE_DATA);
                    }
                    
                    
                    // set ACC and CustomInstruction
                    node->setAcc();
                    node->setCustomInstruction("ACC");
                    // extract pattern of ACC
                    int levels = _LoopexclusiveBBs.size();
                    varType count = 1, interval = 1, repeat = 1;
                    int initialLevel = levels, addLevel;
                    auto initialBB = node->BB();
                    auto addBB = pre->BB();
                    for (int i = 0; i < levels; i++)
                    {
                        if (_LoopexclusiveBBs[i].count(addBB))
                        {
                            addLevel = i;
                        }
                        if (_LoopexclusiveBBs[i].count(initialBB))
                        {
                            initialLevel = i;
                        }
                    }
                    assert(addLevel <= initialLevel);
                    for (int i = 0; i < levels; i++)
                    {
                        if (i < addLevel)
                            interval *= _loopsAffineCounts[i];
                        else if (i <= initialLevel)
                            count *= _loopsAffineCounts[i];
                        else
                            repeat *= _loopsAffineCounts[i];
                    }
                    node->setAccPattern(count, interval, repeat);
                }
            }
        }
        if (node->outputEdges().size() == 0)
        { // if SELECT is dangling
            errs() << node->getName() << " is dangling\n";
            rmnodes.insert(node);
            continue;
        }
    }

    errs()<< "final rm::\n";
    for(auto rm : rmnodes){
        errs() << "remove node: "<<rm->getName()<< "\n";
        delNode(rm);
    }
}

///traverse all initialSels & generate CACC
void LLVMCDFG::generateCACC(std::set<LLVMCDFGNode *> &InitialSels){
    outs() << "----------handle conditional ACC(CACC)\n";
    for (auto node : InitialSels)//check if SELECT nodes left forming conditional select
    {
        LLVMCDFGNode *SelParent1, *ADDNode;
        LLVMCDFGNode *Initial = node->getInputPort(0);
        errs() << "\t" << node->getName() << " has initail\n";
        SelParent1 = node->getInputPort(1);
        errs() << "SelParent1 is " << SelParent1->getName() << "\n";
        for (auto input : node->inputNodes())
        {
            errs() << "input: " << input->getName() << "\n";
        }
        assert(SelParent1 != NULL);
        if (SelParent1->customInstruction() != "SELECT")
        {
            errs() << "\t" << node->getName() << "'s parent lefe is not SLECET\n";
            continue;
        }
        if (!(SelParent1->isOutputBackEdge(node)))
        {
            errs() << "\t" << node->getName() << "'s parent is not backedge next cycle\n";
            continue;
        }
        if (edge(node, SelParent1) == NULL)
        {
            errs() << "\t" << node->getName() << "'s parent do not selct it\n";
            continue;
        }
        /// now locate potential SELECTI(initialized) & its parent is SELECT too & parent to it is backedge
        /// TODO: Is Backedge always here?
        std::vector<LLVMCDFGNode *> temoutputs = node->outputNodes();
        bool findCACC = false;
        LLVMCDFGNode *cycleout;
        LLVMCDFGNode *preADD;
        bool legalexchange = true;
        // list to record nodes in the SELECT backedge cycle
        std::set<LLVMCDFGNode *> detected;
        // std::vector<std::pair<LLVMCDFGNode*, LLVMCDFGNode*>> node2parentRecord;
        for (auto selectout : temoutputs)
        {
            // to recode each node's parent (in detected path)
            std::vector<std::pair<LLVMCDFGNode *, LLVMCDFGNode *>> node2parentRecord;
            legalexchange = true;
            /// TODO:only add?; only single add?
            if (!isADDNode(selectout))
            {                          // only consider serial addition
                legalexchange = false; // TODO:how to detect cycle path RIGHT?
                continue;
            }
            // else if(selectout->instruction()->getOpcode() != Instruction::Add){
            //     legalexchange = false; // TODO:how to detect cycle path RIGHT?
            //     continue;
            // }
            else
            {
                errs() << "tem SELECT out: " << selectout->getName() << "\n";
                std::vector<LLVMCDFGNode *> todetect = selectout->outputNodes();
                for (auto output : todetect)
                {
                    node2parentRecord.push_back(std::make_pair(output, selectout)); // record initial parent pair
                }
                while (!findCACC && !todetect.empty())
                {
                    auto temnode = todetect.back();
                    detected.insert(temnode);
                    todetect.pop_back();
                    if (temnode == SelParent1)
                    { // when find SelParent1 in cycle, its inputnode in path will be preADD
                        findCACC = true;
                        for (auto nodepair : node2parentRecord)
                        {
                            if (nodepair.first == SelParent1)
                            {
                                preADD = nodepair.second;
                                errs() << "find preADD: " << preADD->getName() << "!!!!!!!!!!!!\n";
                                break;
                            }
                        }
                        break;
                    }
                    // else if (temnode->instruction() == NULL)
                    // {
                    //     continue;
                    // }
                    // else if (temnode->instruction()->getOpcode() != Instruction::Add)
                    // {
                    //     // legalexchange = false;
                    //     continue;
                    // }
                    else
                    {
                        if (!isADDNode(temnode)){
                            continue;
                        }
                        for (auto nextoutput : temnode->outputNodes())
                        {
                            node2parentRecord.push_back(std::make_pair(nextoutput, temnode));
                            todetect.push_back(nextoutput);
                        }
                    }
                }
            }
            if (findCACC)
            {
                errs() << "find cycle out: " << selectout->getName() << "!!!!!!!!!!!!\n";
                cycleout = selectout;
                detected.insert(cycleout); // if find cycle, add cycleout into detected list
                break;
            }
        }
        if (findCACC)
        {
            assert(cycleout != NULL && preADD != NULL);
            if (cycleout != preADD && legalexchange)
            { // exchange add oprands
                errs() << "cycleout: " << cycleout->getName() << "; backedge preADD: " << preADD->getName() << "\n";
                LLVMCDFGNode *transoperand;
                int preidx, cycleoutidx;
                for (auto preinput : preADD->inputNodes())
                {
                    errs() << "\ttem backedge preADD's input is: " << preinput->getName() << "\n";
                    if (detected.count(preinput))
                    {
                        errs() << "\t\tdetected: pass\n";
                        continue;
                    }
                    else
                    {
                        errs() << "\t\tnot in cycle: record as transoperand\n";
                        transoperand = preinput;
                        preidx = preADD->getInputIdx(preinput);
                    }
                }
                cycleoutidx = cycleout->getInputIdx(node);
                node->addOutputNode(preADD);
                preADD->addInputNode(node, preidx);
                addEdge(node, preADD, EDGE_TYPE_DATA);

                cycleout->addInputNode(transoperand, cycleoutidx);
                transoperand->addOutputNode(cycleout);
                addEdge(transoperand, cycleout, EDGE_TYPE_DATA);

                node->delOutputNode(cycleout);
                cycleout->delInputNode(node);
                auto temedge = edge(node, cycleout);
                delEdge(temedge);

                transoperand->delOutputNode(preADD);
                preADD->delInputNode(transoperand);
                temedge = edge(transoperand, preADD);
                delEdge(temedge);
            }

            /// transform {2 SELCET & 1 ADD} to CACC
            LLVMCDFGNode *condition = SelParent1->getInputPort(2);
            if (SelParent1->getInputIdx(preADD) == 1)
            { // add CTRLNOT after condition if opidx 1 correspond add result
                auto ctrlnot = addNode("CTRLNOT", condition->BB());
                condition->addOutputNode(ctrlnot);
                ctrlnot->addInputNode(condition, 0);
                addEdge(condition, ctrlnot, EDGE_TYPE_CTRL);
                condition = ctrlnot;
            }
            condition->addOutputNode(node);
            node->addInputNode(condition, 1); // CACC Ctrl Operand idx is 1
            addEdge(condition, node, EdgeType::EDGE_TYPE_CTRL);
            ///set acc_first (reserve node when output both exit after ISEL & condSel) 
            if (node->outputEdges().size() == 2)
            { // only output to addself
                node->setAccFirst();
                SelParent1->delOutputNode(node);
                for (auto result : SelParent1->outputNodes())
                { // connect result out to CACC
                    if (result != node)
                    {
                        node->addOutputNode(result);
                        result->addInputNode(node, result->getInputIdx(SelParent1));
                        addEdge(node, result, EDGE_TYPE_DATA);
                    }
                }
                errs() << " SELECT has no other outputs---delete condSEL: " << SelParent1->getName() << "\n";
                // trans all conection of SelParent1 to SELECT; then delnode
                delNode(SelParent1);
            }
            else if (SelParent1->outputEdges().size() == 1)
            {
                errs() << "SELECT has succ: ";
                for (auto succ : node->outputNodes())
                {
                    errs() << succ->getName() << " ";
                }
                errs() << "\n";
                errs() << "ADD has no other outputs---delete condSEL: " << SelParent1->getName() << "\n";
                delNode(SelParent1);
            }
            else
            {
                delEdge(edge(SelParent1, node));
                errs() << "ADD and SELECT both have other outputs\n";
            }
            

            ADDNode = preADD;
            for (auto addend : ADDNode->inputNodes())
            {
                if (addend != node)
                {
                    addend->addOutputNode(node);
                    node->addInputNode(addend, 0);
                    addEdge(addend, node, EDGE_TYPE_DATA);
                }
            }
            if (ADDNode->outputNodes().size() == 0)
            { /// ADD result only output to SelParent; then delnode
                delNode(ADDNode);
            }
            // handle initial(const: set constVal; others: set 0 as initial, ADD original initial variable)
            if (Initial->hasConst())
            {
                node->setInitial(Initial->constVal());
                delNode(Initial);
            }
            else
            {
                node->setInitial(0);
                auto InitialADD = addNode("ADD", Initial->BB());
                auto ACCOutputs = node->outputNodes();
                for (auto ACCOutput : ACCOutputs)
                {
                    int idx = ACCOutput->getInputIdx(node);
                    delEdge(edge(node, ACCOutput));
                    addEdge(InitialADD, ACCOutput, EDGE_TYPE_DATA);
                    InitialADD->addOutputNode(ACCOutput);
                    ACCOutput->addInputNode(InitialADD, idx);
                }
                InitialADD->addInputNode(Initial, 0);
                Initial->addOutputNode(InitialADD);
                addEdge(Initial, InitialADD, EDGE_TYPE_DATA);
                InitialADD->addInputNode(node, 1);
                node->addOutputNode(InitialADD);
                addEdge(node, InitialADD, EDGE_TYPE_DATA);
            }

            // rename CACC
            node->setCustomInstruction("CACC");
            node->setAcc();
            // extract pattern of ACC
            int levels = _LoopexclusiveBBs.size();
            varType count = 1, interval = 1, repeat = 1;
            int initialLevel = levels, addLevel;
            auto initialBB = node->BB();
            auto addBB = ADDNode->BB();
            for (int i = 0; i < levels; i++)
            {
                if (_LoopexclusiveBBs[i].count(addBB))
                {
                    addLevel = i;
                }
                if (_LoopexclusiveBBs[i].count(initialBB))
                {
                    initialLevel = i;
                }
            }
            assert(addLevel <= initialLevel);
            for (int i = 0; i < levels; i++)
            {
                if (i < addLevel)
                    interval *= _loopsAffineCounts[i];
                else if (i <= initialLevel)
                    count *= _loopsAffineCounts[i];
                else
                    repeat *= _loopsAffineCounts[i];
            }
            node->setAccPattern(count, interval, repeat);
        }
    }
}


void LLVMCDFG::handleSelectNodes(){
    std::map<int, LLVMCDFGNode *> nodes = _nodes;
    std::set<LLVMCDFGNode*> InitialSels;
    std::set<LLVMCDFGNode*> rmnodes;
    std::set<LLVMCDFGNode*> skipSel;
    //find initial SELECT for multi-level loop
    for (auto &elem : _nodes)
    {
        auto node = elem.second;
        if(skipSel.count(node)){
            continue;
        }
        if (node->customInstruction() == "SELECT")
        {
            errs() << "check SELECT: " << node->getName() << "\n";
            errs() << "\t"<< node->getName() << " has inputs: "<< node->inputNodes().size() << ": ";
            bool hasinitial = false;
            LLVMCDFGNode* Initial = NULL;//record INPUT if exist
            auto teminputs = node->inputNodes();
            for (auto pre : teminputs){
                errs() << pre->getName() << " ";
            }
            //input nodes idx problem
            assert(teminputs.size() == 3);
            auto pre = node->getInputPort(1);
            auto pre0 = node->getInputPort(0);
            ///if SELECT node controled by LOOPSTART to InitSELECT
            if (node->getInputPort(2)->customInstruction() == "LOOPSTART")
            {//TODO: consider runtime initial
                Initial = pre0;
                hasinitial = true;
                node->setInitial(0);
                delEdge(edge(node->getInputPort(2), node));
                node->setCustomInstruction("InitSELECT");
                InitialSels.insert(node);
                // extract refresh interval of InitSELECT
                int levels = _LoopexclusiveBBs.size();
                varType interval = 1;
                int initialLevel = levels;
                auto initialBB = node->BB();
                for (int i = 0; i < levels; i++)
                {
                    if (_LoopexclusiveBBs[i].count(initialBB))
                    {
                        initialLevel = i;
                    }
                }
                if(initialLevel == levels - 1){
                    interval = 0;//initial only once
                }
                else{
                    for (int i = 0; i < levels; i++)
                    {
                        if (i <= initialLevel)
                            interval *= _loopsAffineCounts[i];
                    }
                }
                node->setISELInterval(interval);
            }
            //assert(preins != NULL && "what's wrong with backedge");
            //start from initial, find select with LOOPSTART down
            std::map<LLVMCDFGNode *, int> initSels;
            LLVMCDFGNode *initSel = node;
            for (int i = 0; i < _LoopexclusiveBBs.size(); i++)
            {
                if (_LoopexclusiveBBs[i].count(initSel->BB()))
                {
                    initSels[initSel] = i;
                    errs() << "\toriginal initsel of loop " << i << " \n";
                    break;
                }
            }
            while (1)
            {
                bool findinitSel = false;
                for (auto initout : initSel->outputNodes())
                {
                    if (initout->customInstruction() == "SELECT")
                    {
                        if (initout->getInputPort(2)->customInstruction() == "LOOPSTART" && initout->getInputIdx(initSel) == 0)
                        {
                            if (pre->isOutputBackEdge(initout))
                            {
                                errs() << "\t\ttem init SELECT is "<< initout->getName() << "\n";
                                skipSel.insert(initout);
                                rmnodes.insert(initout);
                                auto outputNodes = initout->outputNodes();//copy
                                for(auto succInitout : outputNodes){
                                    int idx = succInitout->getInputIdx(initout);
                                    EdgeType type = edge(initout, succInitout)->type();
                                    delEdge(edge(initout, succInitout));
                                    initSel->addOutputNode(succInitout);
                                    succInitout->addInputNode(initSel, idx);
                                    addEdge(initSel, succInitout, type);
                                }
                                delEdge(edge(initSel, initout));
                                findinitSel = true;
                                break;
                            }
                        }
                    }
                }
                if (!findinitSel)
                {
                    break;
                }
            }
            errs() << "---final initSel: " << node->getName() << "\n";
        }
    }
    for(auto rm : rmnodes){
        errs() << "remove node: "<<rm->getName()<< "\n";
        delNode(rm);
    }
    rmnodes.clear();

    printDOT("afterInitialSel.dot");
    ///TODO: multi reset value
    ///Detect Conditional Dual Initial
    outs() << "------Detect Conditional Dual Initial\n";
    //key(std::pair) 
    //----first:node to transform; 
    //----second:the node corresponding value to Dual Initial
    //value(std::vector) 
    //----LLVMCDFGNodes of contions
    std::map<std::pair<LLVMCDFGNode*, LLVMCDFGNode*>, std::vector<LLVMCDFGNode*>> DualInit2ConditionPath;
    for (auto node : InitialSels){
        //auto node = *&elem;
        LLVMCDFGNode* DualInitValue, *DualInitSel, *DualInitSelPre;
        assert(node->isInputBackEdge(node->getInputPort(1)));
        auto backedgeNode = node->getInputPort(1);
        std::vector<LLVMCDFGNode*> treeNodeStack;
        //analyze SELECT tree from backedge node and record in selectTree
        std::set<LLVMCDFGNode*> selectTree;
        analyzeSelectTree(backedgeNode, node, selectTree);
        selectTree.insert(node);
        outs() << "selectTree has nodes: ";
        for(auto selectTreenode:selectTree){
            outs() << selectTreenode->getName() << " ";
        }
        outs() << "\n";
        treeNodeStack.push_back(backedgeNode);
        bool legal = true;
        bool hasDualInit = false;
        while(!treeNodeStack.empty())
        {
            auto temNode = treeNodeStack.back();
            treeNodeStack.pop_back();
            if(temNode == node)
                continue;
            if(selectTree.count(temNode)){
                if(isADDNode(temNode)){
                    while(temNode->getInputIdx(node) == -1 && legal){
                        LLVMCDFGNode* preADD;
                        LLVMCDFGNode* opNotinTree;
                        int preADDidx;
                        bool Addop0inTree = selectTree.count(temNode->getInputPort(0));
                        bool Addop1inTree = selectTree.count(temNode->getInputPort(1));
                        assert(!(Addop0inTree && Addop1inTree));
                        if(Addop0inTree){
                            preADD = temNode->getInputPort(0);
                            opNotinTree = temNode->getInputPort(1);
                            preADDidx = 0;
                        }else{
                            preADD = temNode->getInputPort(1);
                            opNotinTree = temNode->getInputPort(0);
                            preADDidx = 1;
                        }
                        if(isADDNode(preADD)){
                            LLVMCDFGNode* prepreADD;
                            int prepreADDidx;
                            Addop0inTree = selectTree.count(preADD->getInputPort(0));
                            Addop1inTree = selectTree.count(preADD->getInputPort(1));
                            assert(!(Addop0inTree && Addop1inTree));
                            if(Addop0inTree){
                                prepreADD = preADD->getInputPort(0);
                                prepreADDidx = 0;
                            }else{
                                prepreADD = preADD->getInputPort(1);
                                prepreADDidx = 1;
                            }
                            delEdge(edge(opNotinTree, temNode));
                            delEdge(edge(prepreADD, preADD));
                            temNode->addInputNode(prepreADD, 1-preADDidx);
                            prepreADD->addOutputNode(temNode);
                            addEdge(prepreADD, temNode, EDGE_TYPE_DATA);
                            preADD->addInputNode(opNotinTree, prepreADDidx);
                            opNotinTree->addOutputNode(preADD);
                            addEdge(opNotinTree, preADD, EDGE_TYPE_DATA);
                        }else{
                            legal = false;
                        }
                        selectTree.erase(preADD);
                    }
                    if(temNode->getInputIdx(node) != -1){
                        int inTreeidx = 1-temNode->getInputIdx(node);
                        treeNodeStack.push_back(temNode->getInputPort(inTreeidx));
                    }else{
                        legal = false;
                    }
                }else if(temNode->customInstruction() == "SELECT"){
                    bool Selop0inTree = selectTree.count(temNode->getInputPort(0));
                    bool Selop1inTree = selectTree.count(temNode->getInputPort(1));
                    LLVMCDFGNode* preNode;//in Tree prenode
                    LLVMCDFGNode* opNotinTree;//not in Tree prenode
                    int inTreeidx;
                    if(Selop0inTree && Selop1inTree){
                        treeNodeStack.push_back(temNode->getInputPort(0));
                        treeNodeStack.push_back(temNode->getInputPort(1));
                        continue;
                    }
                    //SELECT has input out of TREE---->conditional DualInit
                    else if(Selop0inTree){
                        preNode = temNode->getInputPort(0);
                        opNotinTree = temNode->getInputPort(1);
                        inTreeidx = 0;
                    }else{
                        preNode = temNode->getInputPort(1);
                        opNotinTree = temNode->getInputPort(0);
                        inTreeidx = 1;
                    }
                    treeNodeStack.push_back(preNode);
                    if(temNode != backedgeNode){
                        assert(temNode->outputNodes().size() == 1);
                    }
                    DualInitValue = opNotinTree;
                    DualInitSel = temNode;
                    DualInitSelPre = preNode;
                    hasDualInit = true;
                }else{
                    legal = false;
                }
            }else{
                continue;
            }
        }//while end
        //if(legal && hasDualInit && hardwaresupport){
        if(legal && hasDualInit){
            auto preDualInitPathNode = DualInitValue;
            auto DualInitPathNode = DualInitSel;
            errs() << "-------conclude DualInitPath Nodes\n";
            while(1){
                errs() << "temp DualInitPath is: " << DualInitPathNode->getName() <<\
                "; outputNodes: " << DualInitPathNode->outputNodes().size() <<"\n";
                int idx = DualInitPathNode->getInputIdx(preDualInitPathNode);
                auto condition = DualInitPathNode->getInputPort(2);
                if (idx == 1){
                    bool hasNot = false;
                    for (auto ctrlnot : condition->outputNodes())
                    {
                        if (ctrlnot->customInstruction() == "CTRLNOT")
                        {
                            condition = ctrlnot;
                            hasNot = true;
                            break;
                        }
                    }
                    if(!hasNot){
                        auto NewCtrlNot = addNode("CTRLNOT", condition->BB());
                        connectNodes(condition, NewCtrlNot, 0, EDGE_TYPE_CTRL);
                        condition = NewCtrlNot;
                    }
                }
                auto DualInitpair = std::make_pair(node, DualInitValue);
                DualInit2ConditionPath[DualInitpair].push_back(condition);
                if(DualInitPathNode == backedgeNode){
                    break;
                }
                assert(DualInitPathNode->outputNodes().size() == 1 && "what's wrong with SELECT tree");
                auto outputNode = DualInitPathNode->outputNodes()[0];
                DualInitPathNode = outputNode;
            }
            //LLVMCDFGNode* DualInitValue, *DualInitSel, *DualInitSelPre;
            if(DualInitSel == backedgeNode){
                for(auto out:DualInitSel->outputNodes()){
                    bool isBackEdge = DualInitSel->isOutputBackEdge(out);
                    int idx = out->getInputIdx(DualInitSel);
                    connectNodes(DualInitSelPre, out, idx, EDGE_TYPE_DATA, isBackEdge);
                }
            }
            else{    
                assert(DualInitSel->outputNodes().size() == 1);
                LLVMCDFGNode* outputNode = DualInitSel->outputNodes()[0];
                int idx = outputNode->getInputIdx(DualInitSel);
                errs() << "tem DualInitSel: " << DualInitSel->getName() << " idx = "<<idx<<"\n";
                connectNodes(DualInitSelPre, outputNode, idx, EDGE_TYPE_DATA);
            }
            delNode(DualInitSel);
        }
    }
    transAddOpSelectTree(); 
    
    generateACC(InitialSels);

    generateCACC(InitialSels);

    printDOT("before_ACC2CACC.dot");
    ///ACC to CACC
    for (auto &elem : _nodes){
        auto node = elem.second;
        if(node->customInstruction() == "ACC"){
            errs() << "tem ACC in is: " << node->getName() << "\n";
            bool findZeroConst = false;
            LLVMCDFGNode* ACCIn = node->inputNodes()[0];
            std::vector<LLVMCDFGNode*> SelsStack;
            std::vector<LLVMCDFGNode*> Conditionpath;
            LLVMCDFGNode* remainPathNode = NULL;
            if(ACCIn->customInstruction() == "SELECT"){
                SelsStack.push_back(ACCIn);
            }
            while (!SelsStack.empty() && !findZeroConst)
            {
                LLVMCDFGNode* temNode = SelsStack.back();
                SelsStack.pop_back();
                errs() << "\ttem SELECT is: " << temNode->getName() << "\n";
                for(int i = 0; i < 2; i++){
                    auto inputNode = temNode->getInputPort(i);
                    if(inputNode->customInstruction() == "SELECT"){
                        SelsStack.push_back(inputNode);
                    }else if(inputNode->customInstruction() == "CONST" && inputNode->constVal() == 0){
                        findZeroConst = true;
                        remainPathNode = inputNode;
                        if(temNode->outputNodes().size() == 1){
                            auto outputNode = temNode->outputNodes()[0];
                            int idx = outputNode->getInputIdx(temNode);
                            rmnodes.insert(temNode);
                            temNode->getInputPort(1-i)->addOutputNode(outputNode);
                            outputNode->addInputNode(temNode->getInputPort(1-i), idx);
                            addEdge(temNode->getInputPort(1-i), outputNode, EDGE_TYPE_DATA);
                            ///set CACC
                            node->setCustomInstruction("CACC");
                        }
                    }
                }
            }
            if(remainPathNode == NULL){
                continue;
            }
            errs() << "-------conclude remainPath Nodes\n";
            while(remainPathNode != ACCIn){
                errs() << "temp remainPathNode is: " << remainPathNode->getName() <<"; outputNodes: " << remainPathNode->outputNodes().size() <<"\n";
                assert(remainPathNode->outputNodes().size() == 1 && "what's wrong with SELECT tree");
                auto temOutput = remainPathNode->outputNodes()[0];
                int idx = temOutput->getInputIdx(remainPathNode);
                auto condition = temOutput->getInputPort(2);
                if (idx == 1){
                    bool hasNot = false;
                    for (auto ctrlnot : condition->outputNodes())
                    {
                        if (ctrlnot->customInstruction() == "CTRLNOT")
                        {
                            condition = ctrlnot;
                            hasNot = true;
                            break;
                        }
                    }
                    if(!hasNot){
                        auto NewCtrlNot = addNode("CTRLNOT", condition->BB());
                        connectNodes(condition, NewCtrlNot, 0, EDGE_TYPE_CTRL);
                        condition = NewCtrlNot;
                    }
                }
                remainPathNode = temOutput;
                Conditionpath.push_back(condition);
            }
            assert(Conditionpath.size() > 0);
            LLVMCDFGNode* finalCond;
            if(Conditionpath.size() == 1){
                finalCond = Conditionpath[0];
            }else{
                LLVMCDFGNode* NewCtrlAnd;
                LLVMCDFGNode* and0 = Conditionpath[0];
                LLVMCDFGNode* and1 = Conditionpath[1];
                for(int i = 1; i < Conditionpath.size(); i++){
                    NewCtrlAnd = addNode("CTRLAND", node->BB());
                    NewCtrlAnd->addInputNode(and0, 0);
                    and0->addOutputNode(NewCtrlAnd);
                    addEdge(and0, NewCtrlAnd, EDGE_TYPE_CTRL);
                    NewCtrlAnd->addInputNode(and1, 1);
                    and1->addOutputNode(NewCtrlAnd);
                    addEdge(and1, NewCtrlAnd, EDGE_TYPE_CTRL);
                    and0 = NewCtrlAnd;
                    and1 = Conditionpath[i+1];
                }
                finalCond = NewCtrlAnd;
            }
            LLVMCDFGNode* finalNot = NULL;
            for (auto ctrlnot : finalCond->outputNodes())
            {
                if (ctrlnot->customInstruction() == "CTRLNOT")
                {
                    finalNot = ctrlnot;
                    break;
                }
            }
            if(finalNot == NULL){
                finalNot = addNode("CTRLNOT", node->BB());
                finalCond->addOutputNode(finalNot);
                finalNot->addInputNode(finalCond, 0);
                addEdge(finalCond, finalNot, EDGE_TYPE_CTRL);
                finalCond = finalNot;
            }
            node->addInputNode(finalCond, 2);
            finalCond->addOutputNode(node);
            addEdge(finalCond, node, EDGE_TYPE_CTRL);
        }
    }
    for(auto rmnode:rmnodes){
        delNode(rmnode);
    }
    rmnodes.clear();

    printDOT("brforeCIACC.dot");
    ///ACC/CACC to CIACC
    errs() << "--------transform ACC/CACC to CIACC\n";
    for(auto &elem : DualInit2ConditionPath){
        auto node = elem.first.first;
        auto DualInitValue = elem.first.second;
        auto Conditionpath = elem.second;
        errs() << "temNode is: " << node->getName() << "\n";
        assert(node->customInstruction() == "ACC" || node->customInstruction() == "CACC");
        assert(Conditionpath.size() > 0);
        LLVMCDFGNode* finalCond;
        if(Conditionpath.size() == 1){
            finalCond = Conditionpath[0];
        }else{
            LLVMCDFGNode* NewCtrlAnd;
            LLVMCDFGNode* and0 = Conditionpath[0];
            LLVMCDFGNode* and1 = Conditionpath[1];
            for(int i = 1; i < Conditionpath.size(); i++){
                NewCtrlAnd = addNode("CTRLAND", node->BB());
                NewCtrlAnd->addInputNode(and0, 0);
                and0->addOutputNode(NewCtrlAnd);
                addEdge(and0, NewCtrlAnd, EDGE_TYPE_CTRL);
                NewCtrlAnd->addInputNode(and1, 1);
                and1->addOutputNode(NewCtrlAnd);
                addEdge(and1, NewCtrlAnd, EDGE_TYPE_CTRL);
                and0 = NewCtrlAnd;
                and1 = Conditionpath[i+1];
            }
            finalCond = NewCtrlAnd;
        }
        connectNodes(finalCond, node, 3, EDGE_TYPE_CTRL);
        connectNodes(DualInitValue, node, 1, EDGE_TYPE_DATA);
        node->setCustomInstruction("CIACC");
    }
}

    // generate MAC nodes
    void LLVMCDFG::genMACNodes(){
        std::map<int, LLVMCDFGNode *> nodes = _nodes;
        for(auto &elem : nodes){
            LLVMCDFGNode* node = elem.second;
            if(node->customInstruction() == "ACC"){
                auto inNode = node->getInputPort(0);
                unsigned int opcode = -1;
                if(inNode->instruction() != NULL){
                    opcode = inNode->instruction()->getOpcode();
                }else if(inNode->customInstruction() == "MUL"){
                    opcode = Instruction::Mul;
                }
                if(opcode == Instruction::Mul || opcode == Instruction::FMul){
                    auto op0 = inNode->getInputPort(0);
                    auto op1 = inNode->getInputPort(1);
                    errs() << "ACC is: " << node->getName() <<", MUL is: "<< inNode->getName() << "\n";
                    if(inNode->outputNodes().size() == 1)
                        delNode(inNode);
                    else
                        delEdge(edge(inNode, node));
                    node->addInputNode(op0, 0);
                    op0->addOutputNode(node);
                    addEdge(op0, node, EDGE_TYPE_DATA);
                    node->addInputNode(op1, 1);
                    op1->addOutputNode(node);
                    addEdge(op1, node, EDGE_TYPE_DATA);
                    node->setCustomInstruction("MAC");
                }
            }
        }
    }

// print DOT file of CDFG
void LLVMCDFG::printDOT(std::string fileName) {
#ifndef DUMP_TMP_CDFG
    return;
#endif
	std::ofstream ofs;
	ofs.open(fileName.c_str());
    ofs << "Digraph G {\n";
    // nodes
	assert(_nodes.size() != 0);
    for(auto &elem : _nodes){
        auto node = elem.second;
        auto name = node->getName();
        auto ins = node->instruction();
        ofs << name << "[label = \"" << name;
        //add address name of LSU
        if(ins != NULL){
            if(dyn_cast<LoadInst>(ins) || dyn_cast<StoreInst>(ins))
            ofs << ", ArrayName=" << node->getLSArrayName();
            //errs() << "sueecss land\n";
        }
        if(node->isLSaffine()){
            ofs << ", Stride=[ ";
            auto stride = node->getLSstride();
            for(int i = 0; i < stride.size(); i++){
                ofs << i<< ", " << stride[i] <<"; ";
            }
            ofs << "], Bounds=[ ";
            auto LSbounds = node->getLSbounds();
            for(int dim = 0; dim < 1; dim++){
                ofs << "(" << LSbounds[0].to_string()<<", "<<LSbounds[1].to_string()<<")*"<<LSbounds[2].to_string()<<"; ";
            }
            ofs << "]";
        }
        if(node->hasConst()){
            ofs << ", Const=" << node->constVal();
        }
        ofs << "\", shape = box, color = black];\n";
    }
	// edges
    std::map<std::pair<LLVMCDFGNode*, LLVMCDFGNode*>, int> edgestack;    
    for(auto &elem : _edges){
        auto edge = elem.second;
        auto srcName = edge->src()->getName();
        auto dstName = edge->dst()->getName();
        ofs << srcName << " -> " << dstName;
        auto type = edge->type();
        if(type == EDGE_TYPE_CTRL){
            ofs << "[color = red";
        }else if(type == EDGE_TYPE_MEM){
            ofs << "[color = blue";
        }else{
            ofs << "[color = black";
        }
        bool isBackEdge = edge->src()->isOutputBackEdge(edge->dst());
        if(isBackEdge){
            ofs << ", style = dashed";
        }else{
            ofs << ", style = bold";
        }
        int opIdx = edge->dst()->getInputIdx(edge->src());
        auto pair = std::make_pair(edge->src(), edge->dst());
        if(edgestack.count(pair)){
            opIdx = 1 - edgestack[pair];
        }else{
            edgestack[pair] = opIdx;
        }
        ofs << ", label = \"Op=" << opIdx;
        if(type == EDGE_TYPE_MEM){
            ofs << ", iterdist = " << edge->dst()->getSrcDep(edge->src()).distance;
        }
        ofs << "\"];\n";
    }
	ofs << "}\n";
	ofs.close();
}

void LLVMCDFG::printHierarchyDOT(std::string fileName) {
#ifndef DUMP_TMP_CDFG
    return;
#endif
	std::ofstream ofs;
	ofs.open(fileName.c_str());
    ofs << "digraph G {\n";
    // nodes
	assert(_nodes.size() != 0);
    int levels = nestloops().size();
    for(int level = levels-1; level >= 0; level--){
        ofs << "subgraph cluster_" << level << "{\n";
        ofs << "style = \"dashed\";\n";
        ofs << "label = \"level" << level <<"\";\n";
        for(auto &elem : _nodes){
            auto node = elem.second;
            auto name = node->getName();
            auto ins = node->instruction();
            if(!_LoopexclusiveBBs[level].count(node->BB())){
                continue;
            }
            ofs << name << "[label = \"" << name;
            //add address name of LSU
            if(ins != NULL){
                if(dyn_cast<LoadInst>(ins) || dyn_cast<StoreInst>(ins))
                ofs << ", ArrayName=" << node->getLSArrayName();
                //errs() << "sueecss land\n";
            }
            if(node->isLSaffine()){
                ofs << ", Stride=[ ";
                auto stride = node->getLSstride();
                for(int i = 0; i < stride.size(); i++){
                    ofs << i<< ", " << stride[i] <<"; ";
                }
                ofs << "], Bounds=[ ";
                auto LSbounds = node->getLSbounds();
                for(int dim = 0; dim < 1; dim++){
                    ofs << "(" << LSbounds[0].to_string()<<", "<<LSbounds[1].to_string()<<")*"<<LSbounds[2].to_string()<<"; ";
                }
                ofs << "]";
            }
            if(node->hasConst()){
                ofs << ", Const=" << node->constVal();
            }
            ofs << "\", shape = box, color = black];\n";
        }
    }
    for(int level = levels-1; level >= 0; level--){
        ofs << "}\n";
    }

	// edges
    std::map<std::pair<LLVMCDFGNode*, LLVMCDFGNode*>, int> edgestack;    
    for(auto &elem : _edges){
        auto edge = elem.second;
        auto srcName = edge->src()->getName();
        auto dstName = edge->dst()->getName();
        ofs << srcName << " -> " << dstName;
        auto type = edge->type();
        if(type == EDGE_TYPE_CTRL){
            ofs << "[color = red";
        }else if(type == EDGE_TYPE_MEM){
            ofs << "[color = blue";
        }else{
            ofs << "[color = black";
        }
        bool isBackEdge = edge->src()->isOutputBackEdge(edge->dst());
        if(isBackEdge){
            ofs << ", style = dashed";
        }else{
            ofs << ", style = bold";
        }
        int opIdx = edge->dst()->getInputIdx(edge->src());
        auto pair = std::make_pair(edge->src(), edge->dst());
        if(edgestack.count(pair)){
            opIdx = 1 - edgestack[pair];
        }else{
            edgestack[pair] = opIdx;
        }
        ofs << ", label = \"Op=" << opIdx;
        if(type == EDGE_TYPE_MEM){
            ofs << ", iterdist = " << edge->dst()->getSrcDep(edge->src()).distance;
        }
        ofs << "\"];\n";
    }
	ofs << "}\n";
	ofs.close();
}

void LLVMCDFG::printAffineDOT(std::string fileName) {
    errs() << "-------------print final affine DOT\n";
	std::ofstream ofs;
    auto loops = _nestloops;
    std::map<std::string, int> minoffset;
    std::map<std::string, int> maxsize;
    //aligent load/store (min) offset
    for(auto &elem : _nodes){
        auto node = elem.second;
        auto ins = node->instruction();
        if(ins != NULL){
            if(node->getLSoffset().index() == 0){
                int LSoffset = std::get<int>(node->getLSoffset().value);
                if(dyn_cast<LoadInst>(ins) || dyn_cast<StoreInst>(ins)){
                    auto arrayname = node->getLSArrayName();
                    if(!minoffset.count(arrayname)){
                        minoffset[arrayname] = LSoffset;
                    }else if(minoffset[arrayname] > LSoffset){
                        minoffset[arrayname] = LSoffset;
                    }
                }
            }
        }
    }
    //aligent load/store (max) size
    for(auto &elem : _nodes){
        auto node = elem.second;
        auto ins = node->instruction();
        if(ins != NULL){
            if(dyn_cast<LoadInst>(ins) || dyn_cast<StoreInst>(ins)){
                auto LoadBound = node->getLSbounds();
                auto arrayname = node->getLSArrayName();
                varType size = 0;
                // if(node->isLSaffine()){
                //     size += LoadBound[1] - LoadBound[0];
                //     size += LoadBound[2];
                // }else{
                size = LoadBound[1]-LoadBound[0]+LoadBound[2];
                if(size.index() == 2){
                    continue;
                }
                //}
                int sizeint = std::get<int>(size.value);
                int LSoffset = std::get<int>(node->getLSoffset().value);
                if(minoffset[arrayname] < LSoffset){
                    sizeint += (LSoffset-minoffset[arrayname]);
                }
                if(!maxsize.count(arrayname)){
                    maxsize[arrayname] = sizeint;
                }else if(maxsize[arrayname] < sizeint){
                    maxsize[arrayname] = sizeint;
                }
            }
        }
    }
	ofs.open(fileName.c_str());
    ofs << "Digraph G {\n";
    // nodes
	assert(_nodes.size() != 0);
    for(auto &elem : _nodes){
        auto node = elem.second;
        auto name = node->getName();
        auto ins = node->instruction();
        ofs << name<<"[opcode=" << node->finalInstruction();
        if(node->isAcc()){
            auto patternmap = node->getAccPattern();
            ofs << ", acc_params=\""<< node->getInitial();
            ofs << ", " << patternmap["count"].to_string();
            ofs << ", " << patternmap["interval"].to_string();      
            ofs << ", " << patternmap["repeat"].to_string() <<"\"";
            ofs << ", acc_first=" << node->isAccFirst();///TODO:here acc_first is always initial
        }
        else if(node->hasConst()){
            ofs << ", label=\""<<name<< ", Const="<< node->constVal() << "\"";
        }
        else if(node->isLSaffine()){
            std::string arrayname = node->getLSArrayName();
            ofs << ", ref_name=\"" << arrayname<<"\"";
            // ofs << "opcode=";
            // if(dyn_cast<LoadInst>(ins))
            //     ofs << "input, ref_name=\"" << arrayname<<"\"";
            // else if(dyn_cast<StoreInst>(ins))
            //     ofs << "output, ref_name=\"" << arrayname<<"\"";
            auto LSbounds = node->getLSbounds();
            varType size = 0;
            varType offset;
            varType reduceoffset;
            if(minoffset.count(arrayname) && (dyn_cast<LoadInst>(ins) || dyn_cast<StoreInst>(ins))){
                offset = minoffset[arrayname];
            }else{
                offset = node->getLSoffset();
            }
            if(maxsize.count(arrayname) && (dyn_cast<LoadInst>(ins) || dyn_cast<StoreInst>(ins))){
                size = maxsize[arrayname];
            }
            else{
                size += (LSbounds[1] - LSbounds[0]);
                size += LSbounds[2];
            }
            if(offset % BITWIDTH == 0){
                //offset = node->getLSoffset();
                reduceoffset = node->getLSstart() - offset;
            }else{
                size += offset%BITWIDTH;
                offset = offset-offset%BITWIDTH;
                reduceoffset = node->getLSstart() - offset;
            }
            errs()<<arrayname<<" LSstart: "<<node->getLSstart().to_string()<<"\n";
            ofs << ", size=\""<<size.to_string()<<"\"";
            ofs << ", offset=\"" << offset.to_string() <<", ";
            //assert(node->getLSstart()>=0);
            if(node->getLSstart() == "")///LS totally fixed
                reduceoffset = node->getLSoffset()%BITWIDTH; 
            errs()<<arrayname<<" node->getLSoffset(): "<<node->getLSoffset().to_string()<<"\n";
            errs()<<arrayname<<" reduceoffset: "<<reduceoffset.to_string()<<"\n";
            ofs <<reduceoffset.to_string()<<"\"";
            ofs << ", pattern=\"";
            auto stride = node->getLSstride();
            varType resetstride = 0;
            for(int i = 0; i < stride.size(); i++){
                //if(i == 0 || stride[i] == 0)
                //    ofs << stride[i] <<", " << getLoopsAffineCounts(i);
                //else
                ofs << (stride[i]-resetstride).to_string() <<", " << getLoopsAffineCounts(i).to_string();
                resetstride += (getLoopsAffineCounts(i)-1)*stride[i];
                if(i < stride.size()-1)
                    ofs << ", ";
            }
            ofs << "\"";
        }
        // else if(node->customInstruction() == "SELECT"){
        //     ofs << "opcode=sel";
        // }
        else if(node->customInstruction() == "InitSELECT"){
            ofs << ", interval=" << node->getISELInterval().to_string();
        }
        else if(node->customInstruction() != ""){
            if(node->hasConst()){
                ofs << ", value=" << node->constVal();
            }
        }
        else if(ins != NULL){
            if(dyn_cast<LoadInst>(ins) || dyn_cast<StoreInst>(ins)){
                auto LSBounds = node->getLSbounds();
                auto LoopsCounts = getLoopsAffineCounts();
                varType size = 0;
                varType offset;
                varType reduceoffset;
                varType cycle = 1;
                std::string arrayname = node->getLSArrayName();
                for(int level = 0; level < LoopsCounts.size(); level++){
                    cycle *= LoopsCounts[level];
                }
                if(minoffset.count(arrayname)){
                    ///TODO: if right?
                    if(node->getLSoffset().index() != 2){
                        int LSoffset = std::get<int>(node->getLSoffset().value);
                        if(LSoffset > minoffset[arrayname]){
                            offset = minoffset[arrayname];
                        }else{
                            offset = LSoffset;
                        }
                    }
                }else{
                    offset = node->getLSoffset();
                }
                if(maxsize.count(arrayname)){
                    size = maxsize[arrayname] - offset;
                }else{ 
                    size = (LSBounds[1]-LSBounds[0]+LSBounds[2]) - offset;
                }
                if((offset%BITWIDTH).index() == 0){
                    int remainder = std::get<int>((offset%BITWIDTH).value);
                    if(remainder == 0){
                        //offset = node->getLSoffset();
                        reduceoffset = 0;
                    }else{
                        size += remainder;
                        reduceoffset = remainder;
                        offset = offset - remainder;
                    }

                }else{
                    size += offset%BITWIDTH;
                    reduceoffset = offset%BITWIDTH;
                    offset = offset-offset%BITWIDTH;
                }
                ofs << ", ref_name=\"" << arrayname<<"\"";
                ofs << ", size=\""<<size.to_string()<<"\"";
                ofs << ", offset=\""<<offset.to_string()<<", "<<reduceoffset.to_string()<<"\"";
                ofs << ", cycles=\""<<cycle.to_string()<<"\"";
            }
        }
        ofs << "];\n";
        //ofs << ", shape = box, color = black];\n";
    }
	// edges
    std::map<std::pair<LLVMCDFGNode*, LLVMCDFGNode*>, int> edgestack;    
    for(auto &elem : _edges){
        auto edge = elem.second;
        auto srcName = edge->src()->getName();
        auto dstName = edge->dst()->getName();
        auto type = edge->type();
        ofs << srcName << " -> " << dstName;
        int opIdx = edge->dst()->getInputIdx(edge->src());
        auto pair = std::make_pair(edge->src(), edge->dst());
        if(edgestack.count(pair)){// Square operation
            opIdx = 1 - edgestack[pair];
        }else{
            edgestack[pair] = opIdx;
        }
        ofs << "[operand = " << opIdx;
        if(type == EDGE_TYPE_CTRL){
            ofs << " ,color = red";
        }else if(type == EDGE_TYPE_MEM){
            ofs << " ,color = blue";
        }else{
            ofs << " ,color = black";
        }
        bool isBackEdge = edge->src()->isOutputBackEdge(edge->dst());
        if(isBackEdge){
            ofs << ", style = dashed, constraint=false, backedge=1";
        }else{
            ofs << ", style = bold";
        }
        ofs << ", label = \"Op=" << opIdx;
        if(type == EDGE_TYPE_MEM){
            ofs << ", iterdist = " << edge->dst()->getSrcDep(edge->src()).distance;
        }
        ofs << "\"];\n";
    }
	ofs << "}\n";
	ofs.close();
}

//print subgraph as a cluster in a WHOLE DFG
void LLVMCDFG::printAsSubTask(std::ofstream &ofs, int kernel) {
    outs() << "------print Sub Task kernel " << kernel << "-------\n";
    auto loops = _nestloops;
    std::string kernelStr = std::to_string(kernel);
    std::map<std::string, int> minoffset;
    std::map<std::string, int> maxsize;
    //aligent load (min) offset
    for(auto &elem : _nodes){
        auto node = elem.second;
        auto ins = node->instruction();
        if(ins != NULL){
            if(node->getLSoffset().index() == 0){
                int LSoffset = std::get<int>(node->getLSoffset().value);
                if(dyn_cast<LoadInst>(ins)){
                    auto arrayname = node->getLSArrayName();
                    if(!minoffset.count(arrayname)){
                        minoffset[arrayname] = LSoffset;
                    }else if(minoffset[arrayname] > LSoffset){
                        minoffset[arrayname] = LSoffset;
                    }
                }
            }
        }
    }
    //aligent load (max) size
    for(auto &elem : _nodes){
        auto node = elem.second;
        auto ins = node->instruction();
        if(ins != NULL){
            if(dyn_cast<LoadInst>(ins)){
                auto LoadBound = node->getLSbounds();
                auto arrayname = node->getLSArrayName();
                varType size = 0;
                // if(node->isLSaffine()){
                //     size += LoadBound[1] - LoadBound[0];
                //     size += LoadBound[2];
                // }else{
                size = LoadBound[1]-LoadBound[0]+LoadBound[2];
                if(size.index() == 2){
                    continue;
                }
                //}
                int sizeint = std::get<int>(size.value);
                int LSoffset = std::get<int>(node->getLSoffset().value);
                if(minoffset[arrayname] < LSoffset){
                    size += (LSoffset-minoffset[arrayname]);
                }
                if(!maxsize.count(arrayname)){
                    maxsize[arrayname] = sizeint;
                }else if(maxsize[arrayname] < sizeint){
                    maxsize[arrayname] = sizeint;
                }
            }
        }
    }
    ofs << "subgraph cluster_" + kernelStr + " {\n";
    ofs << "label = \"subgraph_" + kernelStr + "\";\n";
    // nodes
	assert(_nodes.size() != 0);
    for(auto &elem : _nodes){
        auto node = elem.second;
        auto name = node->getName()+"_"+std::to_string(kernel);
        auto ins = node->instruction();
        ofs << name<<"[opcode=" << node->finalInstruction();
        if(node->isAcc()){
            auto patternmap = node->getAccPattern();
            ofs << ", acc_params=\""<< node->getInitial();
            ofs << ", " << patternmap["count"].to_string();
            ofs << ", " << patternmap["interval"].to_string();      
            ofs << ", " << patternmap["repeat"].to_string() <<"\"";
            ofs << ", acc_first=" << node->isAccFirst();///TODO:here acc_first is always initial
        }
        else if(node->isLSaffine()){
            std::string arrayname = node->getLSArrayName();
            ofs << ", ref_name=\"" << arrayname<<"\"";
            // ofs << "opcode=";
            // if(dyn_cast<LoadInst>(ins))
            //     ofs << "input, ref_name=\"" << arrayname<<"\"";
            // else if(dyn_cast<StoreInst>(ins))
            //     ofs << "output, ref_name=\"" << arrayname<<"\"";
            auto LSbounds = node->getLSbounds();
            varType size = 0;
            varType offset;
            varType reduceoffset;
            if(minoffset.count(arrayname) && dyn_cast<LoadInst>(ins)){
                offset = minoffset[arrayname];
            }else{
                offset = node->getLSoffset();
            }
            if(maxsize.count(arrayname) && dyn_cast<LoadInst>(ins)){
                size = maxsize[arrayname];
            }
            else{
                size += (LSbounds[1] - LSbounds[0]);
                size += LSbounds[2];
            }
            if(offset % BITWIDTH == 0){
                //offset = node->getLSoffset();
                reduceoffset = node->getLSstart() - offset;
            }else{
                size += offset%BITWIDTH;
                offset = offset-offset%BITWIDTH;
                reduceoffset = node->getLSstart() - offset;
            }
            errs()<<arrayname<<" LSstart: "<<node->getLSstart().to_string()<<"\n";
            ofs << ", size=\""<<size.to_string()<<"\"";
            ofs << ", offset=\"" << offset.to_string() <<", ";
            //assert(node->getLSstart()>=0);
            if(node->getLSstart() == "")///LS totally fixed
                reduceoffset = node->getLSoffset()%BITWIDTH; 
            errs()<<arrayname<<" node->getLSoffset(): "<<node->getLSoffset().to_string()<<"\n";
            errs()<<arrayname<<" reduceoffset: "<<reduceoffset.to_string()<<"\n";
            ofs <<reduceoffset.to_string()<<"\"";
            ofs << ", pattern=\"";
            auto stride = node->getLSstride();
            varType resetstride = 0;
            for(int i = 0; i < stride.size(); i++){
                //if(i == 0 || stride[i] == 0)
                //    ofs << stride[i] <<", " << getLoopsAffineCounts(i);
                //else
                ofs << (stride[i]-resetstride).to_string() <<", " << getLoopsAffineCounts(i).to_string();
                resetstride += (getLoopsAffineCounts(i)-1)*stride[i];
                if(i < stride.size()-1)
                    ofs << ", ";
            }
            ofs << "\"";
        }
        // else if(node->customInstruction() == "SELECT"){
        //     ofs << "opcode=sel";
        // }
        else if(node->customInstruction() != ""){
            if(node->hasConst()){
                ofs << ", value=" << node->constVal();
            }
        }
        else if(ins != NULL){
            if(dyn_cast<LoadInst>(ins) || dyn_cast<StoreInst>(ins)){
                auto LSBounds = node->getLSbounds();
                auto LoopsCounts = getLoopsAffineCounts();
                varType size = 0;
                varType offset;
                varType reduceoffset;
                varType cycle = 1;
                std::string arrayname = node->getLSArrayName();
                for(int level = 0; level < LoopsCounts.size(); level++){
                    cycle *= LoopsCounts[level];
                }
                if(minoffset.count(arrayname) && dyn_cast<LoadInst>(ins)){
                    ///TODO: if right?
                    if(node->getLSoffset().index() != 2){
                        int LSoffset = std::get<int>(node->getLSoffset().value);
                        if(LSoffset > minoffset[arrayname]){
                            offset = minoffset[arrayname];
                        }else{
                            offset = LSoffset;
                        }
                    }
                }else{
                    offset = node->getLSoffset();
                }
                if(maxsize.count(arrayname) && dyn_cast<LoadInst>(ins)){
                    size = maxsize[arrayname] - offset;
                }else{ 
                    size = (LSBounds[1]-LSBounds[0]+LSBounds[2]) - offset;
                }
                if((offset%BITWIDTH).index() == 0){
                    int remainder = std::get<int>((offset%BITWIDTH).value);
                    if(remainder == 0){
                        //offset = node->getLSoffset();
                        reduceoffset = 0;
                    }else{
                        size += remainder;
                        reduceoffset = remainder;
                        offset = offset - remainder;
                    }

                }else{
                    size += offset%BITWIDTH;
                    reduceoffset = offset%BITWIDTH;
                    offset = offset-offset%BITWIDTH;
                }
                ofs << ", ref_name=\"" << arrayname<<"\"";
                ofs << ", size=\""<<size.to_string()<<"\"";
                ofs << ", offset=\""<<offset.to_string()<<", "<<reduceoffset.to_string()<<"\"";
                ofs << ", cycles=\""<<cycle.to_string()<<"\"";
            }
        }
        ofs << "];\n";
        //ofs << ", shape = box, color = black];\n";
    }
	// edges
    std::map<std::pair<LLVMCDFGNode*, LLVMCDFGNode*>, int> edgestack;    
    for(auto &elem : _edges){
        auto edge = elem.second;
        auto srcName = edge->src()->getName()+"_"+std::to_string(kernel);
        auto dstName = edge->dst()->getName()+"_"+std::to_string(kernel);
        auto type = edge->type();
        ofs << srcName << " -> " << dstName;
        int opIdx = edge->dst()->getInputIdx(edge->src());
        auto pair = std::make_pair(edge->src(), edge->dst());
        if(edgestack.count(pair)){// Square operation
            opIdx = 1 - edgestack[pair];
        }else{
            edgestack[pair] = opIdx;
        }
        ofs << "[operand = " << opIdx;
        if(type == EDGE_TYPE_CTRL){
            ofs << " ,color = red";
        }else if(type == EDGE_TYPE_MEM){
            ofs << " ,color = blue";
        }else{
            ofs << " ,color = black";
        }
        bool isBackEdge = edge->src()->isOutputBackEdge(edge->dst());
        if(isBackEdge){
            ofs << ", style = dashed, constraint=false, backedge=1";
        }else{
            ofs << ", style = bold";
        }
        ofs << ", label = \"Op=" << opIdx;
        if(type == EDGE_TYPE_MEM){
            ofs << ", iterdist = " << edge->dst()->getSrcDep(edge->src()).distance;
        }
        ofs << "\"];\n";
    }
	ofs << "}\n";
}

// generate CDFG
void LLVMCDFG::generateCDFG()
{
    outs() << "########################################################\n";
    outs() << "Generate CDFG Started\n";
    // initialized CDFG
    printDOT(_name + "_init.dot");

    auto nestloops = this->nestloops();
    errs() << nestloops.size() << "\n";

    outs() << ">>>>>> add edge between two nodes that have memory dependence (loop-carried)\n";
    addMemDepEdges();
    printDOT(_name + "_after_addMemDepEdges.dot");

    LoopIdxAnalyze();

    // Connect control dependent node pairs among BBs
    outs() << ">>>>>> Connect control dependent node pairs among BBs\n";
    connectCtrlDepBBs();
    printDOT(_name + "_after_connectCtrlDepBBs.dot");

    // insert Control NOT node behind the node with FALSE_COND control output edge
    outs() << ">>>>>> Insert CTRLNOT node behind the node with FALSE_COND control output edge\n";
    insertCtrlNotNodes();
    printDOT(_name + "_after_insertCtrlNotNodes.dot");
    // transfer the multiple control predecessors (input nodes) into a inverted OR tree 
    // with the root connected to a node and leaves connected to control predecessors
    outs() << ">>>>>> Transfer multiple control predecessors (input nodes) into a inverted OR tree\n";
    createCtrlOrTree();
    printDOT(_name + "_after_createCtrlOrTree.dot");

    outs() << ">>>>>> Transfer GEP node to MUL/ADD/Const tree\n";
    handleGEPNodes();
    printDOT(_name + "_after_handleGEPNodes.dot");

    if(!_noPattern){
        affineAnalyze();
        outs() << ">>>>>> affine Access Node\n";
        handleAffineLSNodes(); 
        printDOT(_name + "_after_handleAffineLSNodes.dot");
    }

    outs() << ">>>>>> Transfer PHINode to SELECT nodes\n";
    handlePHINodes(); 
    printDOT(_name + "_after_handlePHINodes.dot");

    if(!_noACC){
        transAddOpSelectTree();
        printDOT(_name + "_after_transAddOpSelectTree.dot");

        outs() << ">>>>>> Handle select Node\n";
        handleSelectNodes(); 
        printDOT(_name + "_after_handleSelectNodes.dot");

        outs() << ">>>>>> Generate MAC Node\n";
        genMACNodes(); 
        //printDOT(_name + "_after_genMACNodes.dot");
    }

    outs() << ">>>>>> Remove redundant nodes at first time\n";
    removeRedundantNodes();

    outs() << ">>>>>> add mask AND node behind the Shl node with bytewidth less than MAX_DATA_BYTE\n";
    addMaskAndNodes();

    outs() << ">>>>>> handle conversion operations including trunc, zext, sext\n";
    handleConversionNodes();
    printDOT(_name + "_after_handleConversionNodes.dot");

    outs() << ">>>>>> fuse constant input node with the mul output node for acc node\n";
    fuseAccConstNode();
    printDOT(_name + "_after_fuseAccConstNode.dot");

    outs() << ">>>>>> simplify the control logic\n";
    simplifyCtrlLogic();
    printDOT(_name + "_after_simplifyCtrlLogic.dot");

    outs() << ">>>>>> Remove redundant nodes\n";
    removeRedundantNodes();
    printDOT(_name + "_after_removeRedundantNodes.dot");


    outs() << ">>>>>> Assign final node name\n";
    assignFinalNodeName();
    //printDOT(_name + "_after_assignFinalNodeName.dot");
    printAffineDOT("CDFG.dot");
    //printAffineDOT("affine_"+_name + ".dot");

    outs() << "Generate CDFG Ended\n";
    outs() << "########################################################\n";


}