//! \file
/*
**  Copyright (C) - Triton
**
**  This program is under the terms of the LGPLv3 License.
*/

#ifndef TRITON_SYMBOLICENGINE_H
#define TRITON_SYMBOLICENGINE_H

#include <list>
#include <map>
#include <string>

#include "ast.hpp"
#include "astDictionaries.hpp"
#include "memoryOperand.hpp"
#include "registerOperand.hpp"
#include "symbolicEnums.hpp"
#include "symbolicExpression.hpp"
#include "symbolicOptimization.hpp"
#include "symbolicSimplification.hpp"
#include "symbolicVariable.hpp"
#include "tritonTypes.hpp"



//! \module The Triton namespace
namespace triton {
/*!
 *  \addtogroup triton
 *  @{
 */

  //! \module The Engines namespace
  namespace engines {
  /*!
   *  \ingroup triton
   *  \addtogroup engines
   *  @{
   */

    //! \module The Symbolic Execution namespace
    namespace symbolic {
    /*!
     *  \ingroup engines
     *  \addtogroup symbolic
     *  @{
     */

      //! \class SymbolicEngine
      /*! \brief The symbolic engine class. */
      class SymbolicEngine
        : public triton::engines::symbolic::SymbolicSimplification,
          public triton::engines::symbolic::SymbolicOptimization,
          public triton::ast::AstDictionaries {

        protected:

          //! Enable / Disable flag.
          bool enableFlag;

          /*! \brief Defines if we use a symbolic emulation.
           *
           * \description
           * **true**: full symbolic execution (emulation).
           * **false**: concolic execution.
           */
          bool emulationFlag;

          //! Number of registers
          triton::uint32 numberOfRegisters;

          //! Symbolic expressions id.
          triton::__uint uniqueSymExprId;

          //! Symbolic variables id.
          triton::__uint uniqueSymVarId;

          /*! \brief The map of symbolic variables
           *
           * \description
           * **item1**: variable id<br>
           * **item2**: symbolic variable
           */
          std::map<triton::__uint, SymbolicVariable*> symbolicVariables;

          /*! \brief The map of symbolic expressions
           *
           * \description
           * **item1**: symbolic reference id<br>
           * **item2**: symbolic expression
           */
          std::map<triton::__uint, SymbolicExpression*> symbolicExpressions;

          /*! \brief map of address -> symbolic expression
           *
           * \description
           * **item1**: memory address<br>
           * **item2**: symbolic reference id
           */
          std::map<triton::__uint, triton::__uint> memoryReference;

          /*! \brief map of <address:size> -> symbolic expression.
           *
           * \description
           * **item1**: <addr:size><br>
           * **item2**: symbolic reference id
           */
          std::map<std::pair<triton::__uint, triton::uint32>, triton::ast::AbstractNode*> alignedMemoryReference;

          //! \brief The logical conjunction vector of path constraint.
          std::vector<triton::ast::AbstractNode*> pathConstraint;


        public:

          //! Symbolic register state.
          triton::__uint* symbolicReg;

          //! Creates a new symbolic expression.
          SymbolicExpression* newSymbolicExpression(triton::ast::AbstractNode* node, symkind_e kind, std::string comment="");

          //! Removes the symbolic expression corresponding to the id.
          void removeSymbolicExpression(triton::__uint symExprId);

          //! Removes aligned entry.
          void removeAlignedMemory(triton::__uint addr);

          //! Adds a symbolic variable.
          SymbolicVariable* newSymbolicVariable(symkind_e kind, triton::__uint kindValue, triton::uint32 size, std::string comment="");

          //! Converts a symbolic expression to a symbolic variable. `symVarSize` must be in bits.
          SymbolicVariable* convertExpressionToSymbolicVariable(triton::__uint exprId, triton::uint32 symVarSize, std::string symVarComment="");

          //! Converts a symbolic memory expression to a symbolic variable.
          SymbolicVariable* convertMemoryToSymbolicVariable(triton::arch::MemoryOperand& mem, std::string symVarComment="");

          //! Converts a symbolic register expression to a symbolic variable.
          SymbolicVariable* convertRegisterToSymbolicVariable(triton::arch::RegisterOperand& reg, std::string symVarComment="");

          //! Returns the symbolic variable corresponding to the symbolic variable id.
          SymbolicVariable* getSymbolicVariableFromId(triton::__uint symVarId);

          //! Returns the symbolic variable corresponding to the symbolic variable name.
          SymbolicVariable* getSymbolicVariableFromName(std::string& symVarName);

          //! Returns the symbolic expression id corresponding to the memory address.
          triton::__uint getSymbolicMemoryId(triton::__uint addr);

          //! Returns the symbolic expression corresponding to the id.
          SymbolicExpression* getSymbolicExpressionFromId(triton::__uint symExprId);

          //! Returns the map of symbolic registers defined.
          std::map<triton::arch::RegisterOperand, SymbolicExpression*> getSymbolicRegisters(void);

          //! Returns the map of symbolic memory defined.
          std::map<triton::__uint, SymbolicExpression*> getSymbolicMemory(void);

          //! Returns the symbolic expression id corresponding to the register.
          triton::__uint getSymbolicRegisterId(triton::arch::RegisterOperand& reg);

          //! Returns the symbolic memory value.
          triton::uint8 getSymbolicMemoryValue(triton::__uint address);

          //! Returns the symbolic memory value.
          triton::uint128 getSymbolicMemoryValue(triton::arch::MemoryOperand& mem);

          //! Returns the symbolic register value.
          triton::uint128 getSymbolicRegisterValue(triton::arch::RegisterOperand& reg);

          //! Returns an immediate symbolic operand.
          triton::ast::AbstractNode* buildSymbolicImmediateOperand(triton::arch::ImmediateOperand& imm);

          //! Returns a symbolic memory operand.
          triton::ast::AbstractNode* buildSymbolicMemoryOperand(triton::arch::MemoryOperand& mem);

          //! Returns a symbolic register operand.
          triton::ast::AbstractNode* buildSymbolicRegisterOperand(triton::arch::RegisterOperand& reg);

          //! Returns the new symbolic memory expression expression and links this expression to the instruction.
          SymbolicExpression* createSymbolicMemoryExpression(triton::arch::Instruction& inst, triton::ast::AbstractNode* node, triton::arch::MemoryOperand& mem, std::string comment="");

          //! Returns the new symbolic register expression expression and links this expression to the instruction.
          SymbolicExpression* createSymbolicRegisterExpression(triton::arch::Instruction& inst, triton::ast::AbstractNode* node, triton::arch::RegisterOperand& reg, std::string comment="");

          //! Returns the new symbolic flag expression expression and links this expression to the instruction.
          SymbolicExpression* createSymbolicFlagExpression(triton::arch::Instruction& inst, triton::ast::AbstractNode* node, triton::arch::RegisterOperand& flag, std::string comment="");

          //! Returns the new symbolic volatile expression expression and links this expression to the instruction.
          SymbolicExpression* createSymbolicVolatileExpression(triton::arch::Instruction& inst, triton::ast::AbstractNode* node, std::string comment="");

          //! Returns an unique symbolic expression id.
          triton::__uint getUniqueSymExprId(void);

          //! Returns an unique symbolic variable id.
          triton::__uint getUniqueSymVarId(void);

          //! Assigns a symbolic expression to a register.
          void assignSymbolicExpressionToRegister(SymbolicExpression *se, triton::arch::RegisterOperand& reg);

          //! Assigns a symbolic expression to a memory.
          void assignSymbolicExpressionToMemory(SymbolicExpression *se, triton::arch::MemoryOperand& mem);

          //! Returns the full AST of a root node.
          triton::ast::AbstractNode* getFullAst(triton::ast::AbstractNode* node);

          //! Returns the list of the tainted symbolic expressions.
          std::list<SymbolicExpression*> getTaintedSymbolicExpressions(void);

          //! Returns all symbolic expressions.
          std::map<triton::__uint, SymbolicExpression*>& getSymbolicExpressions(void);

          //! Returns all symbolic variables.
          std::map<triton::__uint, SymbolicVariable*>& getSymbolicVariables(void);

          //! Returns all variable declarations representation.
          std::string getVariablesDeclaration(void);

          //! Returns the logical conjunction vector of path constraint.
          std::vector<triton::ast::AbstractNode*>& getPathConstraint(void);

          //! Returns the logical conjunction AST of path constraint.
          triton::ast::AbstractNode* getPathConstraintAst(void);

          //! Add a path constraint.
          void addPathConstraint(triton::ast::AbstractNode* pc);

          //! Adds a symbolic memory reference.
          void addMemoryReference(triton::__uint mem, triton::__uint id);

          //! Concretizes all symbolic memory references.
          void concretizeAllMemory(void);

          //! Concretizes all symbolic register references.
          void concretizeAllRegister(void);

          //! Concretizes a specific symbolic memory reference.
          void concretizeMemory(triton::arch::MemoryOperand& mem);

          //! Concretizes a specific symbolic memory reference.
          void concretizeMemory(triton::__uint addr);

          //! Concretizes a specific symbolic register reference.
          void concretizeRegister(triton::arch::RegisterOperand& reg);

          /*! \brief Enables or disables the symbolic emulation.
           *
           * \description
           * **true**: full symbolic execution (emulation).
           * **false**: concolic execution.
           */
          void emulation(bool flag);

          //! Enables or disables the symbolic execution engine.
          void enable(bool flag);

          //! Returns true if the we perform a full symbolic emulation.
          bool isEmulationEnabled(void);

          //! Returns true if the symbolic execution engine is enabled.
          bool isEnabled(void);

          //! Returns true if the symbolic expression ID exists.
          bool isSymbolicExpressionIdExists(triton::__uint symExprId);

          //! Initializes a SymbolicEngine.
          void init(const SymbolicEngine& other);

          //! Copies a SymbolicEngine.
          void operator=(const SymbolicEngine& other);

          //! Constructor.
          SymbolicEngine();

          //! Constructor by copy.
          SymbolicEngine(const SymbolicEngine& copy);

          //! Destructor.
          ~SymbolicEngine();

      };

    /*! @} End of symbolic namespace */
    };
  /*! @} End of engines namespace */
  };
/*! @} End of triton namespace */
};

#endif /* TRITON_SYMBOLICENGINE_H */

