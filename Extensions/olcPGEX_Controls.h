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

	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
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
		const enum Orientation { VERTICAL, HORIZONTAL, AUTO };

		olc::PixelGameEngine* pge;

		void Init(olc::PixelGameEngine* pixelGameEngine)
		{
			pge = pixelGameEngine;
		}

		class BasicControl;

		struct CBoundingBox
		{
			int x;
			int y;
			int width;
			int height;
		};

		CBoundingBox* CreateBoundingBox(int x, int y, int width, int height)
		{
			return new CBoundingBox{ x, y, width, height };
		}

		/*
		Handles the events of BasicControls
		*/
		class CEventHandler
		{
		public:
			virtual void CClicked(olc::PixelGameEngine* pge, BasicControl* control) {}
			virtual void CReleased(olc::PixelGameEngine* pge, BasicControl* control) {}
			virtual void CHover(olc::PixelGameEngine* pge, BasicControl* control) {}
			virtual void CValueChanged(olc::PixelGameEngine* pge, BasicControl* control, float newValue) {}
		};

		class BasicControl
		{
		public:
			std::vector<CEventHandler*> eventHandlers;
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
			virtual void UpdateControl() {}
			void AddEventHandler(CEventHandler* evh)
			{
				eventHandlers.push_back(evh);
			}
			bool IsInBounds(int x, int y, int w, int h, int pointX, int pointY)
			{
				if (pointX > x && pointX <= x + w && pointY >= y && pointY <= y + h) {
					return true;
				}
				return false;
			}
			bool IsInBounds(CBoundingBox* boundingBox, int pointX, int pointY)
			{
				return IsInBounds(boundingBox->x, boundingBox->y, boundingBox->width, boundingBox->height, pointX, pointY);
			}
			void H_CClicked(BasicControl* ctrl)
			{
				for (CEventHandler* handler : eventHandlers)
					handler->CClicked(pge, ctrl);
			}
			void H_CReleased(BasicControl* ctrl)
			{
				for (CEventHandler* handler : eventHandlers)
					handler->CReleased(pge, ctrl);
			}
			void H_CHover(BasicControl* ctrl)
			{
				for (CEventHandler* handler : eventHandlers)
					handler->CHover(pge, ctrl);
			}
			void H_CValueChanged(BasicControl* ctrl, float newValue)
			{
				for (CEventHandler* handler : eventHandlers)
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
			float textXOffset;
			float textYOffset;
			olc::Pixel backgroundColor;
			olc::Pixel backgroundColorHover;
			olc::Pixel foregroundColor;
			olc::Pixel foregroundColorHover;
			std::string text;
			CButton(olc::vf2d position = olc::vf2d(0.0f, 0.0f), olc::vf2d size = olc::vf2d(0.0f, 0.0f), std::string text = "CButton", olc::Pixel backgroundColor = olc::Pixel(44, 62, 80), olc::Pixel backgroundColorHover = olc::Pixel(52, 73, 94), olc::Pixel foregroundColor = olc::Pixel(236, 240, 241), olc::Pixel foregroundColorHover = olc::Pixel(236, 240, 241))
			{
				this->x = position.x;
				this->y = position.y;
				this->width = size.x;
				this->height = size.y;
				this->backgroundColor = backgroundColor;
				this->backgroundColorHover = backgroundColorHover;
				this->foregroundColor = foregroundColor;
				this->foregroundColorHover = foregroundColorHover;
				this->text = text;
				this->textXOffset = 0;
				this->textYOffset = 0;
			}
			void PlaceText(float tpx, float tpy)
			{
				textXOffset = tpx;
				textYOffset = tpy;
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
				olc::Pixel backgroundColor = this->backgroundColor;
				olc::Pixel foregroundColor = this->foregroundColor;

				CBoundingBox* boundingBox = CreateBoundingBox(x, y, width, height);
				if (IsInBounds(boundingBox, pge->GetMouseX(), pge->GetMouseY())) {
					backgroundColor = this->backgroundColorHover;
					foregroundColor = this->foregroundColorHover;
					H_CHover(this);
				}
				pge->FillRect(x, y, width, height, backgroundColor);
				pge->DrawString(x + textXOffset, y + textYOffset, this->text, foregroundColor, 2);

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
				delete boundingBox;
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
			olc::Pixel backgroundColor;
			olc::Pixel foregroundColor;
			Orientation orientation;
			CProgressBar(olc::vf2d position = olc::vf2d(0.0f, 0.0f), olc::vf2d size = olc::vf2d(0.0f, 0.0f), Orientation orientation = Orientation::HORIZONTAL, olc::Pixel backgroundColor = olc::Pixel(88, 92, 92), olc::Pixel foregroundColor = olc::Pixel(39, 174, 96))
			{
				this->x = position.x;
				this->y = position.y;
				this->width = size.x;
				this->height = size.y;
				this->value = 0.0f;
				this->backgroundColor = backgroundColor;
				this->foregroundColor = foregroundColor;

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
			/* Gets the progress size of horizontal progressbar */
			float GetProgressWidth()
			{
				return (this->width / 100) * value;
			}
			/* Gets the progress size of vertical progressbar */
			float GetProgressHeight()
			{
				return (this->width / 100) * value;
			}
			Orientation GetOrientation()
			{
				return this->orientation;
			}
			void Increment(float incrementValue)
			{
				value += incrementValue;
				H_CValueChanged(this, value);
			}
			/* Only Increments if value in range */
			void SafeIncrement(float incrementValue)
			{
				if (!IsFull())
					Increment(incrementValue);
			}
			void Decrement(float decrementValue) {
				this->value -= decrementValue;
				H_CValueChanged(this, value);
			}
			/* Only Decrements if value in range */
			void SafeDecrement(float decrementValue)
			{
				if (!IsEmpty())
					Decrement(decrementValue);
			}
			void SetValue(float newValue)
			{
				if (newValue != value)
				{
					H_CValueChanged(this, value);
				}
				value = newValue;
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
				CBoundingBox* boundingBox = CreateBoundingBox(x, y, width, height);
				if (IsInBounds(boundingBox, pge->GetMouseX(), pge->GetMouseY()))
				{
					H_CHover(this);
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						H_CClicked(this);
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						H_CReleased(this);
				}
				float newWidth = this->width;
				float newHeight = this->height;
				switch (orientation)
				{
				case HORIZONTAL:
					newWidth = (this->width / 100) * this->value;
					break;
				case VERTICAL:
					newHeight = (this->height / 100) * this->value;
					break;
				}
				pge->FillRect(x, y, this->width, this->height, backgroundColor);
				pge->FillRect(x, y, newWidth, newHeight, foregroundColor);
				delete boundingBox;
			}
		};

		class CSlider : public BasicControl
		{
		public:
			float x;
			float y;
			float size;
			float headOffset;
			olc::Pixel backgroundColor;
			olc::Pixel foregroundColor;
			Orientation orientation;
			bool IsSelected = false;
			CSlider(olc::vf2d position = olc::vf2d(0.0f, 0.0f), float size = 0.0f, Orientation orientation = AUTO, olc::Pixel backgroundColor = olc::Pixel(88, 92, 92), olc::Pixel foregroundColor = olc::Pixel(39, 174, 96))
			{
				this->x = position.x;
				this->y = position.y;
				this->size = size;
				this->headOffset = 0;
				this->backgroundColor = backgroundColor;
				this->foregroundColor = foregroundColor;

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
					CBoundingBox* boundingBox = CreateBoundingBox(this->x + headOffset, this->y - 30 / 2, 10, 30);
					if (IsInBounds(boundingBox, pge->GetMouseX(), pge->GetMouseY()))
					{
						H_CHover(this);
						if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						{
							IsSelected = true;
							H_CClicked(this);
						}
						if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						{
							H_CReleased(this);
						}
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						IsSelected = false;
					}
					if (IsSelected)
					{
						int newOffset = pge->GetMouseX() - x;
						if (newOffset >= -1 && newOffset <= (int)this->size + 1)
						{
							this->headOffset = newOffset;
							H_CValueChanged(this, headOffset);
						}
					}
					pge->FillRect(this->x, this->y, this->size, 5, this->backgroundColor);
					pge->FillRect(this->x + this->headOffset, this->y - 30 / 2, 10, 30, this->foregroundColor);
					delete boundingBox;
					return;
				}

				if (orientation == VERTICAL)
				{
					CBoundingBox* boundingBox = CreateBoundingBox(this->x - 30 / 2, this->y + headOffset, 30, 10);
					if (IsInBounds(boundingBox, pge->GetMouseX(), pge->GetMouseY()))
					{
						H_CHover(this);
						if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						{
							IsSelected = true;
							H_CClicked(this);
						}
						if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						{
							H_CReleased(this);
						}
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						IsSelected = false;
					}
					if (IsSelected)
					{
						int newOffset = pge->GetMouseY() - y;
						if (newOffset >= -1 && newOffset <= (int)this->size + 1)
						{
							this->headOffset = newOffset;
							H_CValueChanged(this, headOffset);
						}
					}
					pge->FillRect(this->x, this->y, 5, this->size, this->backgroundColor);
					pge->FillRect(this->x - 30 / 2, this->y + headOffset, 30, 10, this->foregroundColor);
					delete boundingBox;
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
			float textXOffset = 0;
			float textYOffset = 0;
			wint_t textScale;
			std::string text;
			olc::Pixel outlineColor;
			olc::Pixel selectionColor;
			olc::Pixel textColor;
			bool Checked = false;
			CCheckBox(olc::vf2d position = olc::vf2d(0, 0), std::string text = "CCheckBox", wint_t textScale = 2, olc::Pixel outlineColor = olc::Pixel(236, 240, 241), olc::Pixel selectionColor = olc::Pixel(231, 76, 60), olc::Pixel textColor = olc::Pixel(236, 240, 241))
			{
				this->x = position.x;
				this->y = position.y;
				this->text = text;
				this->outlineColor = outlineColor;
				this->selectionColor = selectionColor;
				this->textColor = textColor;
				this->textScale = textScale;
			}
			void PlaceText(float textXOffset, float textYOffset) {
				this->textXOffset = textXOffset;
				this->textYOffset = textYOffset;
			}
			float GetX() override
			{
				return this->x;
			}
			float GetY() override
			{
				return this->y;
			}
			bool IsChecked() {
				return Checked;
			}
			void Toggle()
			{
				Checked = !Checked;
			}
			void UpdateControl() override
			{
				CBoundingBox* boundingBox = CreateBoundingBox(x, y, 20, 20);
				if (IsInBounds(boundingBox, pge->GetMouseX(), pge->GetMouseY())) {
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed) {
						Checked = !Checked;
						float newValue;
						switch (Checked) {
						case true:
							newValue = 1;
							break;
						case false:
							newValue = 0;
							break;
						}
						H_CClicked(this);
						H_CValueChanged(this, newValue);
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased) {
						H_CReleased(this);
					}
				}

				if (Checked)
					pge->DrawString(this->x + 3, this->y + 3, "X", this->selectionColor, 2);
				pge->DrawRect(this->x, this->y, 20, 20, this->outlineColor);
				pge->DrawString(this->x + this->textXOffset, this->y + this->textYOffset, this->text, textColor, this->textScale);
				delete boundingBox;
			}
		};

		/* Similar to a slider but in a circle */
		class CSpinner : public BasicControl
		{
		private:
			olc::Pixel outlineColor;
			olc::Pixel dotColor;
			olc::Pixel backgroundColor;
			int x;
			int y;
			int width;
			int height;
			int dotSize;
			float angle = 0.0f;
			float radius = 20;
			int32_t lastValue = 0;
		public:
			CSpinner(olc::vf2d position = olc::vf2d(0, 0), float radius = 0.0f, float dotSize = 0.0f, olc::Pixel outlineColor = olc::Pixel(236, 240, 241), olc::Pixel dotColor = olc::Pixel(46, 204, 113), olc::Pixel bgColor = olc::Pixel(0, 0, 0))
			{
				this->outlineColor = outlineColor;
				this->dotColor = dotColor;
				this->backgroundColor = backgroundColor;
				this->radius = radius;
				this->x = position.x;
				this->y = position.y;
				this->dotSize = dotSize;
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
				CBoundingBox* boundingBox = CreateBoundingBox(x - radius, y - radius, radius * 2, radius * 2);
				if (IsInBounds(boundingBox, pge->GetMouseX(), pge->GetMouseY()))
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
				pge->FillCircle(x, y, radius, backgroundColor);
				pge->DrawCircle(x, y, radius, outlineColor);
				pge->FillCircle(x + radius / 2 * sin(angle), y + radius / 2 * cos(angle), dotSize, dotColor);
				delete boundingBox;
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
			std::vector<CCheckBox*> checkboxVector;
			CPool_Handler(std::vector<CCheckBox*> checkboxVector = {})
			{
				this->checkboxVector = checkboxVector;
			}
			void AddItem(CCheckBox* checkbox)
			{
				this->checkboxVector.push_back(checkbox);
			}
			void SetItems(std::vector<CCheckBox*> checkboxVector)
			{
				this->checkboxVector = checkboxVector;
			}
			void CValueChanged(olc::PixelGameEngine* pge, BasicControl* ctrl, float nVal) override
			{
				for (CCheckBox* cb : checkboxVector)
				{
					cb->Checked = false;
				}
				((CCheckBox*)ctrl)->Checked = true;
			}
		};

		/* The easiest way of setting up a CPool_Handler */
		CPool_Handler* SetupPool(std::vector<CCheckBox*> checkboxVector)
		{
			CPool_Handler* poolHandler = new CPool_Handler();
			poolHandler->checkboxVector = checkboxVector;
			for (CCheckBox* cb : checkboxVector)
			{
				cb->AddEventHandler(poolHandler);
			}
			return poolHandler;
		}

	}
}
#endif
