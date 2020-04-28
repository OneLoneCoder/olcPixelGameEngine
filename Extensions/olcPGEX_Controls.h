/*
	olcPGEX_Controls.h
	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                       Controls v1.0                         |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	basic controls (For now buttons, progress bars and sliders) and event
	handling.
	NOTE: This version may still contain some bugs

	Example
	~~~~~~~

	#define OLC_PGE_APPLICATION
	#include "olcPixelGameEngine.h"

	#define OLC_PGEX_CONTROLS
	#include "olcPGEX_Controls.h"

	float r = 0.0f, g = 0.0f, b = 0.0f;
	class ControlsTest : public olc::PixelGameEngine
	{
	public:
		ControlsTest()
		{
		}
	private:
		olc::ctrls::CSlider red;
		olc::ctrls::CSlider green;
		olc::ctrls::CSlider blue;
	public:

		bool OnUserCreate() override
		{
			olc::ctrls::CSlider red(this, olc::vf2d(200, 200), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
			this->red = red;

			olc::ctrls::CSlider green(this, olc::vf2d(200, 250), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
			this->green = green;

			olc::ctrls::CSlider blue(this, olc::vf2d(200, 300), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
			this->blue = blue;
			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{
			Clear(olc::BLACK);
			this->red.UpdateControl();
			this->green.UpdateControl();
			this->blue.UpdateControl();

			FillRect(690, 190, 110, 120, olc::Pixel(r, g, b));
			DrawRect(690, 190, 110, 120, olc::WHITE);

			r = this->red.Value(255);
			g = this->green.Value(255);
			b = this->blue.Value(255);
			return true;
		}
	};

	int main()
	{
		ControlsTest demo;
		if (demo.Construct(1100, 500, 1, 1))
			demo.Start();
		return 0;
	}
*/

#define DEFAULT_RGB olc::Pixel(255, 255, 255)
#ifdef OLC_PGEX_CONTROLS
#undef OLC_PGEX_CONTROLS
namespace olc
{
	namespace ctrls
	{
		const enum Orientation{VERTICAL, HORIZONTAL, AUTO};

		struct CBoundingBox
		{
			int x;
			int y;
			int width;
			int height;
		};

		CBoundingBox* CreateBoundingBox(int x, int y, int cw, int ch)
		{
			return new CBoundingBox{ x, y, cw, ch };
		}

		/*
		Handles the events of BasicControls
		*/
		class CEventHandler
		{
		public:
			virtual void CClicked(olc::PixelGameEngine* olcpge) {} // Evertything
			virtual void CReleased(olc::PixelGameEngine* olcpge) {} // Everything
			virtual void CHover(olc::PixelGameEngine* olcpge) {} // Everything
			virtual void CValueChanged(olc::PixelGameEngine* olcpge, float newValue) {} // Progressbar, Slider
		};

		/*
		Basic functions, that every Control needs
		*/
		class BasicControl
		{
		public:
			std::vector<CEventHandler*> hndlrs;
			olc::PixelGameEngine* pge;
			virtual float GetX()
			{
				return 0.0f;
			}

			virtual float GetY()
			{
				return 0.0f;
			}

			virtual float GetWidth()
			{
				return 0.0f;
			}

			virtual float GetHeight()
			{
				return 0.0f;
			}
			virtual void UpdateControl()
			{
			}
			void AddEventHandler(CEventHandler* evh)
			{
				hndlrs.push_back(evh);
			}
			bool IsInBounds(int x, int y, int w, int h, int px, int py)
			{
				if (px > x && px <= x + w && py >= y && py <= y + h) {
					return true;
				}
				return false;
			}
		};


		/*
		--------------------------------------------------
		        All controls are registered below
		--------------------------------------------------
		*/

		class CButton : public BasicControl
		{
		public:
			float x;
			float y;
			float width;
			float height;
			float textPosX;
			float textPosY;
			olc::Pixel bg_color;
			olc::Pixel bg_color_hover;
			olc::Pixel fg_color;
			olc::Pixel fg_color_hover;
			std::string text;
			CButton(olc::PixelGameEngine* pge = nullptr, olc::vf2d pos = olc::vf2d(0.0f, 0.0f), olc::vf2d size = olc::vf2d(0.0f, 0.0f), std::string text = "CButton", olc::Pixel bg_color = DEFAULT_RGB, olc::Pixel bg_color_hover = DEFAULT_RGB, olc::Pixel fg_color = DEFAULT_RGB, olc::Pixel fg_color_hover = DEFAULT_RGB)
			{
				this->x = pos.x;
				this->y = pos.y;
				this->width = size.x;
				this->height = size.y;
				this->bg_color = bg_color;
				this->bg_color_hover = bg_color_hover;
				this->fg_color = fg_color;
				this->fg_color_hover = fg_color_hover;
				this->pge = pge;
				this->text = text;
				this->textPosX = 0;
				this->textPosY = 0;
			}
			void PlaceText(float tpx, float tpy)
			{
				textPosX = tpx;
				textPosY = tpy;
			}
			float GetX() override
			{
				return this->x;
			}
			float GetY() override
			{
				return this->y;
			}
			float GetWidth() override
			{
				return this->width;
			}
			float GetHeight() override
			{
				return this->height;
			}
			void UpdateControl() override
			{
				olc::Pixel* bgc = &bg_color;
				olc::Pixel* fgc = &fg_color;
				if (IsInBounds(x, y, width, height, pge->GetMouseX(), pge->GetMouseY())) {
					bgc = &bg_color_hover;
					fgc = &fg_color_hover;
				}
				pge->FillRect(x, y, width, height, *bgc);
				pge->DrawString(x + textPosX, y + textPosY, this->text, *fgc, 2);

				for (auto& ev : hndlrs)
				{
					if (IsInBounds(x, y, width, height, pge->GetMouseX(), pge->GetMouseY()))
					{
						if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						{
							ev->CClicked(pge);
						}
						if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						{
							ev->CReleased(pge);
						}
						ev->CHover(pge);
					}
				}
			}
		};

		class CProgressBar : public BasicControl
		{
		public:
			float x;
			float y;
			float width;
			float height;
			float value;
			olc::Pixel bg_color;
			olc::Pixel fg_color;
			Orientation orientation;
			CProgressBar(olc::PixelGameEngine* pge = nullptr, olc::vf2d pos = olc::vf2d(0.0f, 0.0f), olc::vf2d size = olc::vf2d(0.0f, 0.0f), Orientation orientation = Orientation::HORIZONTAL, olc::Pixel bg_color = DEFAULT_RGB, olc::Pixel fg_color = DEFAULT_RGB)
			{
				this->pge = pge;
				this->x = pos.x;
				this->y = pos.y;
				this->width = size.x;
				this->height = size.y;
				this->value = 0.0f;
				this->bg_color = bg_color;
				this->fg_color = fg_color;

				if (orientation == AUTO)
					if (width > height)
						orientation = HORIZONTAL;
					else if (width < height)
						orientation = VERTICAL;
					else
						orientation = HORIZONTAL;

				this->orientation = orientation;
			}
			float GetX() override
			{
				return this->x;
			}
			float GetY() override
			{
				return this->y;
			}
			float GetWidth() override
			{
				return this->width;
			}
			float GetHeight() override
			{
				return this->height;
			}
			/* Gets the progress size of horizontally oritented progessbar */
			float GetProgressWidth()
			{
				return (this->width / 100) * value;
			}
			/* Gets the progress size of vertically oriented progressbar */
			float GetProgressHeight()
			{
				return (this->width / 100) * value;
			}
			Orientation GetOrientation()
			{
				return this->orientation;
			}
			void Increment(float v)
			{
				value += v;
				for (auto& e : hndlrs)
				{
					e->CValueChanged(pge, value);
				}
			}
			/* Only Increments if value in range */
			void SafeIncrement(float v)
			{
				if (!IsFull())
					this->value += v;
			}
			void Decrement(float v) {
				this->value -= v;
				for (auto& e : hndlrs)
				{
					e->CValueChanged(this->pge, this->value);
				}
			}
			/* Only Decrements if value in range */
			void SafeDecrement(float v)
			{
				if (!IsEmpty())
					this->value -= v;
			}
			bool IsFull()
			{
				return value >= 100;
			}
			bool IsEmpty()
			{
				return value <= 0;
			}
			void UpdateControl() override
			{
				for (auto& e : hndlrs)
				{
					if (IsInBounds(this->x, this->y, this->width, this->height, pge->GetMouseX(), pge->GetMouseY()))
					{
						e->CHover(this->pge);
						if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
							e->CClicked(this->pge);
						if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
							e->CReleased(this->pge);
					}
				}
				float nw = this->width;
				float nh = this->height;
				switch (orientation)
				{
				case HORIZONTAL:
					nw = (this->width / 100) * this->value;
						break;
				case VERTICAL:
					nh = (this->height / 100) * this->value;
					break;
				}
				pge->FillRect(x, y, this->width, this->height, bg_color);
				pge->FillRect(x, y, nw, nh, fg_color);
			}
		};

		class CSlider : public BasicControl
		{
		public:
			float x;
			float y;
			float size;
			float headOffset;
			olc::Pixel guide_color;
			olc::Pixel head_color;
			Orientation orientation;
			bool bSelected = false;
			CSlider(olc::PixelGameEngine* pge = nullptr, olc::vf2d pos = olc::vf2d(0.0f, 0.0f), float size = 0.0f, Orientation orientation = AUTO, olc::Pixel guide_color = DEFAULT_RGB, olc::Pixel head_color = DEFAULT_RGB)
			{
				this->pge = pge;
				this->x = pos.x;
				this->y = pos.y;
				this->size = size;
				this->headOffset = 0;
				this->guide_color = guide_color;
				this->head_color = head_color;

				if (orientation == AUTO)
					orientation = HORIZONTAL;

				this->orientation = orientation;
			}
			float GetX() override
			{
				return this->x;
			}
			float GetY() override
			{
				return this->y;
			}
			float GetWidth() override
			{
				return this->size;
			}
			float GetHeight() override
			{
				return this->size;
			}
			void SetHeadOffset(float hoffset)
			{
				this->headOffset = hoffset;
			}
			float GetHeadOffset()
			{
				return this->headOffset;
			}
			float GetPercent()
			{
				return headOffset / (size / 100);
			}
			float Value(float maxv)
			{
				return (maxv / 100) * GetPercent();
			}
			void UpdateControl() override
			{
				if (orientation == HORIZONTAL)
				{
					CBoundingBox* cbb = CreateBoundingBox(this->x + headOffset, this->y - 30/2, 10, 30);
					if (IsInBounds(cbb->x, cbb->y, cbb->width, cbb->height, pge->GetMouseX(), pge->GetMouseY()))
					{
						if (pge->GetMouse(0).bHeld)
						{
							bSelected = true;
						}
					}
					if (pge->GetMouse(0).bReleased)
					{
						bSelected = false;
					}
					if (bSelected)
					{
						int newOffset = pge->GetMouseX() - x;
						if (newOffset >= -1 && newOffset <= (int)this->size + 1)
						{
							this->headOffset = newOffset;
							for (auto& o : hndlrs)
							{
								o->CValueChanged(pge, headOffset);
							}
						}
					}
					pge->FillRect(this->x, this->y, this->size, 5, this->guide_color);
					pge->FillRect(this->x + this->headOffset, this->y - 30/2, 10, 30, this->head_color);
					delete cbb;
					return;
				}

				if (orientation == VERTICAL)
				{
					CBoundingBox* cbb = CreateBoundingBox(this->x - 30/2, this->y + headOffset, 30, 10);
					if (IsInBounds(cbb->x, cbb->y, cbb->width, cbb->height, pge->GetMouseX(), pge->GetMouseY()))
					{
						if (pge->GetMouse(0).bHeld)
						{
							bSelected = true;
						}
					}
					if (pge->GetMouse(0).bReleased)
					{
						bSelected = false;
					}
					if (bSelected)
					{
						int newOffset = pge->GetMouseY() - y;
						if (newOffset >= -1 && newOffset <= (int)this->size + 1)
						{
							this->headOffset = newOffset;
							for (auto& o : hndlrs)
							{
								o->CValueChanged(pge, headOffset);
							}
						}
					}
					pge->FillRect(this->x, this->y, 5, this->size, this->guide_color);
					pge->FillRect(this->x - 30/2, this->y + headOffset, 30, 10, this->head_color);
					delete cbb;
					return;
				}
				return;
			}
		};
	}
}
#undef DEFAULT_RGB
#endif
