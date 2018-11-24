#include "../../inc/ExprNode.hpp"
#include "../../inc/BaseStmtNode.hpp"
#include "../../inc/symtable.hpp"
#include "../../inc/irNode.hpp"
#include "../../inc/utility.hpp"

using namespace::std;

BaseStmtNode::BaseStmtNode(FunctionDeclNode* farther):
    StmtNode(farther)
{}

BaseStmtNode::~BaseStmtNode(){}

AssignStmtNode::AssignStmtNode(FunctionDeclNode* farther):
    BaseStmtNode(farther),to(NULL),from(NULL)
{}

AssignStmtNode::~AssignStmtNode(){}

void AssignStmtNode::update_AST_type(ExprNode* root){
    if(!root->lnode && !root->rnode) return; // this is a Ref Node
    if(root->lnode) update_AST_type(root->lnode);
    if(root->rnode) update_AST_type(root->rnode);

    if(root->lnode->type == "R" || root->rnode->type == "R"){
        root->type = "R"; 
    }
}

vector<IrNode*>& AssignStmtNode::translate(){
    
    update_AST_type(from); // for now
    vector<IrNode*>* code_block = new vector<IrNode*>;
    string res = from->translate(*code_block);
    string type = to->type;

    irBlockInsert(*code_block, new StoreIrNode(type, res, to->name, *(farther->regMan)));
    
    return *code_block;
}


ReadStmtNode::ReadStmtNode(FunctionDeclNode* farther):
    BaseStmtNode(farther){}

ReadStmtNode::~ReadStmtNode(){}

vector<IrNode*>& ReadStmtNode::translate(){
    vector<IrNode*>* code_block = new vector<IrNode*>;
    
    for(auto id:id_list){
        string type = id->type;
        irBlockInsert(*code_block, new ReadIrNode(type, id->name, *(farther->regMan)));
    }
    
    return *code_block;
}


WriteStmtNode::WriteStmtNode(FunctionDeclNode* farther):
    BaseStmtNode(farther){}

WriteStmtNode::~WriteStmtNode(){} 

vector<IrNode*>& WriteStmtNode::translate(){
    vector<IrNode*>* code_block = new vector<IrNode*>;
    
    for(auto id:id_list){
        string type = id -> type;
        irBlockInsert(*code_block, new WriteIrNode(type, id->name, *(farther->regMan)));
    }
    
    return *code_block;
}

ReturnStmtNode::ReturnStmtNode(FunctionDeclNode* farther, ExprNode* expr, int retLoc):
    BaseStmtNode(farther), expr(expr), retLoc(retLoc){}

ReturnStmtNode::~ReturnStmtNode(){}

vector<IrNode*>& ReturnStmtNode::translate(){
    vector<IrNode*>* ir = new vector<IrNode*>;
    string ret = expr->translate(*ir);
    string type = expr->type.substr(0,1);
    string newReg = farther->getNextAvaTemp();

    irBlockInsert(*ir, new StoreIrNode(type, ret, newReg, *(farther->regMan)));
    irBlockInsert(*ir, new StoreIrNode(type, newReg,"$"+to_string(retLoc), *(farther->regMan)));
    irBlockInsert(*ir, new ReturnIrNode(retLoc, *(farther->regMan)));
    
    // mark all global var as live here
    extern Symtable* globalSymtable;
    for(auto kv: globalSymtable->id_map) {
        if(kv.second->isFunc) continue;
        if(kv.second->type == "S") continue;
        ir->back()->insertOutSet(kv.first);
    }

    return *ir;
}
