#pragma once
#include <venera/Parsing/Ast.h>

namespace Venera::Analysis
{
    class Analyser final
    {
    private:
        std::shared_ptr<Parsing::Program> mAst;
        Parsing::FunctionDefinition* mCurrentFunction;

        void AnalyseFunction(std::shared_ptr<Parsing::FunctionDefinition> function);

        bool AnalyseBlock(std::shared_ptr<Parsing::Block> block);
        void AnalyseReturn(std::shared_ptr<Parsing::Return> ret);

        bool TryCoerceLiteral(const std::shared_ptr<Parsing::Expression>& value, const LanguageType& expected);

    public:
        void Analyse();

        Analyser(std::shared_ptr<Parsing::Program> ast);
    };
}