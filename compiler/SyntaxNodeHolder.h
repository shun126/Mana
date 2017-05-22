/*!
mana (compiler)

@author	Shun Moriya
@date	2017-
*/

#pragma once
#include "../runner/common/Setup.h"
#include "SyntaxNode.h"
#include <memory>
#include <vector>

namespace mana
{
	class SyntaxNodeHolder : private Noncopyable
	{
	public:
		SyntaxNodeHolder()
		{
		}

		virtual ~SyntaxNodeHolder()
		{
		}

		void Push(SyntaxNode* node)
		{
			mNodeManager.push_back(std::unique_ptr<SyntaxNode>(node));
		}

	private:
		std::vector<std::unique_ptr<SyntaxNode>> mNodeManager;
	};
}
