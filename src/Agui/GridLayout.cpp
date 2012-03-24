/*   _____                           
 * /\  _  \                     __    
 * \ \ \_\ \      __    __  __ /\_\   
 *  \ \  __ \   /'_ `\ /\ \/\ \\/\ \  
 *   \ \ \/\ \ /\ \_\ \\ \ \_\ \\ \ \ 
 *    \ \_\ \_\\ \____ \\ \____/ \ \_\
 *     \/_/\/_/ \/____\ \\/___/   \/_/
 *                /\____/             
 *                \_/__/              
 *
 * Copyright (c) 2011 Joshua Larouche
 * 
 *
 * License: (BSD)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Agui nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Agui/GridLayout.hpp"
#include <math.h>

namespace agui
{
	GridLayout::GridLayout(void)
		:horizontalSpacing(5),verticalSpacing(5),
		rows(1),columns(1)
	{
	}

	GridLayout::~GridLayout(void)
	{
	}

	void GridLayout::layoutChildren()
	{
		//dividing by zero is rather silly...
		//so we won't let it happen!
		if(rows == 0 && columns == 0)
		{
			return;
		}

		int numChildren = 0;
		for(std::list<Widget*>::iterator it = getChildBegin(); 
			it != getChildEnd(); ++it)
		{
			if((*it)->isVisible())
			{
				numChildren++;
			}
		}

		if(numChildren == 0)
		{
			return;
		}


		int sizeX = 0;
		int sizeY = 0;
		int locationX = 0;
		int locationY = 0;

		int childrenInRow = rows;
		int childrenInColumn = columns;
		if(rows == 0)
		{
			childrenInRow = (int)ceil((double)numChildren / (double)columns);
		}
		else if(columns == 0)
		{
			childrenInColumn = (int)ceil((double)numChildren / (double)rows);
		}

		if(childrenInColumn <= 0)
		{
			childrenInColumn = 1;
		}

		if(childrenInRow <= 0)
		{
			childrenInRow = 1;
		}

		int xCount = 0;
		int yCount = 0;

		for(std::list<Widget*>::iterator it = getChildBegin(); 
			it != getChildEnd(); ++it)
		{

			if(!(*it)->isVisible() && isFilteringVisibility())
			{
				continue;
			}

			//linearly solve for the locations and size
			//this ensures that the spacing is respected

			locationX = (int)(((double)xCount / (double)childrenInColumn) * 
				(double)(getInnerWidth() + verticalSpacing));
			locationY = (int)(((double)yCount / (double)childrenInRow) * 
				(double)(getInnerHeight() + verticalSpacing));

			//solve for next location and subtract it from
			//the current and factor in spacing
			sizeX = (int)((((double)(xCount + 1) / (double)childrenInColumn) * 
				(double)(getInnerWidth() + verticalSpacing)) - 
				locationX - horizontalSpacing);

			sizeY = (int)((((double)(yCount + 1) / (double)childrenInRow) * 
				(double)(getInnerHeight() + verticalSpacing)) -
				locationY - verticalSpacing);
			
			(*it)->setSize(sizeX,sizeY);
			(*it)->setLocation(locationX, locationY);

			xCount++;

			//next row
			if(xCount == childrenInColumn)
			{
				xCount = 0;
				yCount++;
			}

		}

	}

	void GridLayout::setNumberOfRows( int rows )
	{
		if(rows < 0)
		{
			rows = 0;
		}

		this->rows = rows;
		updateLayout();
	}

	void GridLayout::setNumberOfColumns( int columns )
	{
		if(columns < 0)
		{
			columns = 0;
		}
		this->columns = columns;
		updateLayout();
	}

	void GridLayout::setHorizontalSpacing( int spacing )
	{
		horizontalSpacing = spacing;
		updateLayout();
	}

	void GridLayout::setVerticalSpacing( int spacing )
	{
		verticalSpacing = spacing;
	}

	int GridLayout::getNumberOfRows() const
	{
		return rows;
	}

	int GridLayout::getNumberOfColumns() const
	{
		return columns;
	}

	int GridLayout::getHorizontalSpacing() const
	{
		return horizontalSpacing;
	}

	int GridLayout::getVerticalSpacing() const
	{
		return verticalSpacing;
	}

}
