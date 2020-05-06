/*
	olcPGEX_Controls.h
	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                       Controls                              |
	+-------------------------------------------------------------+
	
	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	basic controls and event
	handling.

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
		
			olc::ctrls::Init(this);
			
			olc::ctrls::CSlider red(olc::vf2d(200, 200), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
			this->red = red;

			olc::ctrls::CSlider green(olc::vf2d(200, 250), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
			this->green = green;

			olc::ctrls::CSlider blue(olc::vf2d(200, 300), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
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

#ifdef OLC_PGEX_CONTROLS
#undef OLC_PGEX_CONTROLS

namespace olc
{
	namespace ctrls
	{
		const enum Orientation{VERTICAL, HORIZONTAL, AUTO};
		olc::PixelGameEngine* pge;

		void Init(olc::PixelGameEngine* pxge)
		{
			pge = pxge;
		}

		class BasicControl;

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
			virtual void CClicked(olc::PixelGameEngine* olcpge, BasicControl* cntrl) {} // Evertything
			virtual void CReleased(olc::PixelGameEngine* olcpge, BasicControl* cntrl) {} // Everything
			virtual void CHover(olc::PixelGameEngine* olcpge, BasicControl* cntrl) {} // Everything
			virtual void CValueChanged(olc::PixelGameEngine* olcpge, BasicControl* cntrl, float newValue) {} // Progressbar, Slider
		};

		/*
		Basic functions, that every Control needs
		*/
		class BasicControl
		{
		public:
			std::vector<CEventHandler*> hndlrs;
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
			void H_CClicked(BasicControl* ctrl)
			{
				for (CEventHandler* handler : hndlrs)
					handler->CClicked(pge, ctrl);
			}
			void H_CReleased(BasicControl* ctrl)
			{
				for (CEventHandler* handler : hndlrs)
					handler->CReleased(pge, ctrl);
			}
			void H_CHover(BasicControl* ctrl)
			{
				for (CEventHandler* handler : hndlrs)
					handler->CHover(pge, ctrl);
			}
			void H_CValueChanged(BasicControl* ctrl, float newValue)
			{
				for (CEventHandler* handler : hndlrs)
					handler->CValueChanged(pge, ctrl, newValue);
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
			CButton(olc::vf2d pos = olc::vf2d(0.0f, 0.0f), olc::vf2d size = olc::vf2d(0.0f, 0.0f), std::string text = "CButton", olc::Pixel bg_color = olc::Pixel(44, 62, 80), olc::Pixel bg_color_hover = olc::Pixel(52, 73, 94), olc::Pixel fg_color = olc::Pixel(236, 240, 241), olc::Pixel fg_color_hover = olc::Pixel(236, 240, 241))
			{
				this->x = pos.x;
				this->y = pos.y;
				this->width = size.x;
				this->height = size.y;
				this->bg_color = bg_color;
				this->bg_color_hover = bg_color_hover;
				this->fg_color = fg_color;
				this->fg_color_hover = fg_color_hover;
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
					H_CHover(this);
				}
				pge->FillRect(x, y, width, height, *bgc);
				pge->DrawString(x + textPosX, y + textPosY, this->text, *fgc, 2);

				if (IsInBounds(x, y, width, height, pge->GetMouseX(), pge->GetMouseY()))
				{
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
					{
						H_CClicked(this);
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						H_CReleased(this);
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
			CProgressBar(olc::vf2d pos = olc::vf2d(0.0f, 0.0f), olc::vf2d size = olc::vf2d(0.0f, 0.0f), Orientation orientation = Orientation::HORIZONTAL, olc::Pixel bg_color = olc::Pixel(88, 92, 92), olc::Pixel fg_color = olc::Pixel(39, 174, 96))
			{
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
				H_CValueChanged(this, v);
			}
			/* Only Increments if value in range */
			void SafeIncrement(float v)
			{
				if (!IsFull())
					Increment(v);
			}
			void Decrement(float v) {
				this->value -= v;
				H_CValueChanged(this, v);
			}
			/* Only Decrements if value in range */
			void SafeDecrement(float v)
			{
				if (!IsEmpty())
					Decrement(v);
			}

			void SetValue(float v)
			{
				if (v != value)
				{
					H_CValueChanged(this, v);
				}
				value = v;
			}

			float GetValue()
			{
				return value;
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
				if (IsInBounds(this->x, this->y, this->width, this->height, pge->GetMouseX(), pge->GetMouseY()))
				{
					H_CHover(this);
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						H_CClicked(this);
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						H_CReleased(this);
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
			CSlider(olc::vf2d pos = olc::vf2d(0.0f, 0.0f), float size = 0.0f, Orientation orientation = AUTO, olc::Pixel guide_color = olc::Pixel(88, 92, 92), olc::Pixel head_color = olc::Pixel(39, 174, 96))
			{
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
						H_CHover(this);
						if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						{
							bSelected = true;
							H_CClicked(this);
						}
						if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						{
							H_CReleased(this);
						}
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						bSelected = false;
					}
					if (bSelected)
					{
						int newOffset = pge->GetMouseX() - x;
						if (newOffset >= -1 && newOffset <= (int)this->size + 1)
						{
							this->headOffset = newOffset;
							H_CValueChanged(this, headOffset);
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
						H_CHover(this);
						if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						{
							bSelected = true;
							H_CClicked(this);
						}
						if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						{
							H_CReleased(this);
						}
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						bSelected = false;
					}
					if (bSelected)
					{
						int newOffset = pge->GetMouseY() - y;
						if (newOffset >= -1 && newOffset <= (int)this->size + 1)
						{
							this->headOffset = newOffset;
							H_CValueChanged(this, headOffset);
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

		class CCheckBox : public BasicControl
		{
		public:
			float x;
			float y;
			int box_width = 20;
			int box_height = 20;
			float txt_x = 0;
			float txt_y = 0;
			wint_t txt_scale;
			std::string text;
			olc::Pixel box_color;
			olc::Pixel box_sel;
			olc::Pixel fg_color;
			bool bChecked = false;
			CCheckBox(olc::vf2d pos = olc::vf2d(0, 0), std::string text = "CCheckBox", wint_t txt_scale = 2, olc::Pixel box_color = olc::Pixel(236, 240, 241), olc::Pixel box_sel = olc::Pixel(231, 76, 60), olc::Pixel fg_color = olc::Pixel(236, 240, 241))
			{
				this->x = pos.x;
				this->y = pos.y;
				this->text = text;
				this->box_color = box_color;
				this->box_sel = box_sel;
				this->fg_color = fg_color;
				this->txt_scale = txt_scale;
			}
			void PlaceText(float txt_x, float txt_y) {
				this->txt_x = txt_x;
				this->txt_y = txt_y;
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
				return this->box_width;
			}
			float GetHeight() override
			{
				return this->box_height;
			}
			bool IsChecked() {
				return bChecked;
			}
			void Toggle()
			{
				bChecked = !bChecked;
			}
			void UpdateControl() override
			{
				if (IsInBounds(x, y, box_width, box_height, pge->GetMouseX(), pge->GetMouseY())) {
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed) {
						bChecked = !bChecked;
						float nVal;
						switch (bChecked) {
						case true:
							nVal = 1;
							break;
						case false:
							nVal = 0;
							break;
						}
						H_CClicked(this);
						H_CValueChanged(this, nVal);
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased) {
						H_CReleased(this);
					}
				}

				if (bChecked)
					pge->DrawString(this->x+3, this->y+3, "X", this->box_sel, 2);
				pge->DrawRect(this->x, this->y, this->box_width, this->box_height, this->box_color);
				pge->DrawString(this->x + this->txt_x, this->y + this->txt_y, this->text, fg_color, this->txt_scale);
			}
		};

		/* Similar to a slider but in a circle */
		class CSpinner : public BasicControl
		{
		private:
			olc::Pixel outline_color;
			olc::Pixel dot_color;
			olc::Pixel bg_color;
			int x;
			int y;
			int width;
			int height;
			int dot_size;
			float angle = 0.0f;
			float radius = 20;
			int32_t lastValue = 0;
		public:
			CSpinner(olc::vf2d position = olc::vf2d(0, 0), float radius = 0.0f, float dot_size = 0.0f, olc::Pixel outline_color = olc::Pixel(236, 240, 241), olc::Pixel dot_color = olc::Pixel(46, 204, 113), olc::Pixel bg_color = olc::Pixel(0, 0, 0))
			{
				this->outline_color = outline_color;
				this->dot_color = dot_color;
				this->bg_color = bg_color;
				this->radius = radius;
				this->x = position.x;
				this->y = position.y;
				this->dot_size = dot_size;
			}
			float GetX() override
			{
				return x;
			}
			float GetY() override
			{
				return y;
			}
			float GetPercent()
			{
				return 100 * (angle / 133.0f) + 0.9;
			}
			float Value(float maxv)
			{
				return (maxv / 100) * GetPercent();
			}
			float GetAngle()
			{
				return angle;
			}
			const float MaxAngle()
			{
				return 133.0f;
			}
			void UpdateControl() override
			{
				CBoundingBox* cbb = CreateBoundingBox(x - radius, y - radius, radius * 2, radius * 2);
				if (IsInBounds(cbb->x, cbb->y, cbb->width, cbb->height, pge->GetMouseX(), pge->GetMouseY()))
				{
					H_CHover(this);
					if (lastValue != pge->GetMouseWheel())
					{
						H_CValueChanged(this, GetPercent());
					}
					lastValue = pge->GetMouseWheel();
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
					{
						H_CClicked(this);
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						H_CReleased(this);
					}
					angle += (float)pge->GetMouseWheel() / 20;
				}
				if ((int)GetPercent() > 100)
					angle = 0;
				if (angle < 0)
					angle = 0;
				pge->FillCircle(x, y, radius, bg_color);
				pge->DrawCircle(x, y, radius, outline_color);
				pge->FillCircle(x + radius / 2 * sin(angle), y + radius / 2 * cos(angle), dot_size, dot_color);
			}
		};

		/*
		--------------------------------------------------
				All handlers are registered below
		--------------------------------------------------
		*/

		/* A basic checkbox pool */
		class CPool_Handler : public CEventHandler
		{
		public:
			std::vector<CCheckBox*> cbx;
			CPool_Handler(std::vector<CCheckBox*> cbx = {})
			{
				this->cbx = cbx;
			}
			void AddItem(CCheckBox* cbx)
			{
				this->cbx.push_back(cbx);
			}
			void SetItems(std::vector<CCheckBox*> cbx)
			{
				this->cbx = cbx;
			}
			void CValueChanged(olc::PixelGameEngine* pge, BasicControl* ctrl, float nVal) override
			{
				for (CCheckBox* cb : cbx)
				{
					cb->bChecked = false;
				}
				((CCheckBox*)ctrl)->bChecked = true;
			}
		};

		/* The easiest way of setting up a CPool_Handler */
		CPool_Handler* SetupPool(std::vector<CCheckBox*> cbx)
		{
			CPool_Handler* pool_h = new CPool_Handler();
			pool_h->cbx = cbx;
			for (CCheckBox* cb : cbx)
			{
				cb->AddEventHandler(pool_h);
			}
			return pool_h;
		}

	}
}
#endif
