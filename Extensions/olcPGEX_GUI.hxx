#ifndef __OLCPGEX_GUI_HXX
#define __OLCPGEX_GUI_HXX


//#include "stdafx.h"


namespace olc
{

	class GUI : public olc::PGEX
	{
	public:

		class Widgets
		{
		public:

			void Button();
			void TextBox();
			void ListBox();
			void StaticText();

		};

	public:

		class Events
		{

			bool OnButtonClick();

			bool OnTextBoxClick();
			bool OnTextBoxInput();

			bool OnListBoxClick();
			bool OnListBoxButtonClick();

		};

	};

}


#endif // __OLCPGEX_GUI_HXX
