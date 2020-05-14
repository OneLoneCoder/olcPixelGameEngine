/*
	olcPGEX_Controls.h
	+-------------------------------------------------------------+
	|         OneLoneCoder Pixel Game Engine Extension            |
	|                       Controls                              |
	+-------------------------------------------------------------+

	What is this?
	~~~~~~~~~~~~~
	This is an extension to the olcPixelGameEngine, which provides
	controls, event handler and other things, that help to design a GUI
	using the olcPixelGameEngine.

	Example
	~~~~~~~

	// A basic RGB color picker

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
		olc::ctrls::Slider red;
		olc::ctrls::Slider green;
		olc::ctrls::Slider blue;
	public:

		bool OnUserCreate() override
		{

			olc::ctrls::Initialize(this);

			olc::ctrls::Slider red(olc::vf2d(200, 200), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
			this->red = red;

			olc::ctrls::Slider green(olc::vf2d(200, 250), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
			this->green = green;

			olc::ctrls::Slider blue(olc::vf2d(200, 300), 300, olc::ctrls::HORIZONTAL, olc::GREY, olc::GREEN);
			this->blue = blue;

			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{
			Clear(olc::BLACK);

			this->red.Update();
			this->green.Update();
			this->blue.Update();

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

	What's new?
	~~~~~~~~~~~
	  - Slider looks better
	  - Box is now sensitive to events
	  - You can draw the box as well if you want to
*/

#ifdef OLC_PGEX_CONTROLS
#undef OLC_PGEX_CONTROLS

#define MOUSE_POINT Point(pge->GetMouseX(), pge->GetMouseY())

namespace olc
{
	/* olcPixelGameEngine Controls extension functions & classes */
	namespace ctrls
	{
		const enum Orientation
		{
			VERTICAL, HORIZONTAL, AUTO
		};

		olc::PixelGameEngine* pge;

		void Initialize(olc::PixelGameEngine* pixelGameEngine)
		{
			pge = pixelGameEngine;
		}

		class Point
		{
		public:
			float x;
			float y;
			Point(float x = 0, float y = 0)
			{
				this->x = x;
				this->y = y;
			}
			float GetX()
			{
				return this->x;
			}
			float GetY()
			{
				return this->y;
			}
		};

		class BaseComponent;
		class EventHandler
		{
		public:
			virtual void MouseClicked(olc::PixelGameEngine* pge, BaseComponent* control, Point mouse) {}
			virtual void MouseReleased(olc::PixelGameEngine* pge, BaseComponent* control, Point mouse) {}
			virtual void MouseHover(olc::PixelGameEngine* pge, BaseComponent* control, Point mouse) {}
			virtual void ValueChanged(olc::PixelGameEngine* pge, BaseComponent* control, float newValue) {}
			
			/* Any event on the component has occured */
			virtual void AnyEvent(olc::PixelGameEngine* pge, BaseComponent* control) {}
		};

		class BaseComponent
		{
		public:
			std::vector<EventHandler*> eventHandlers;
			bool lockHandlers = false;
			bool lockUpdates = false;
			int x = 0;
			int y = 0;
			void LockHandlers()
			{
				lockHandlers = true;
			}
			void FreeHandlers()
			{
				lockHandlers = false;
			}
			bool HandlersLocked()
			{
				return lockHandlers;
			}
			void LockUpdates()
			{
				lockUpdates = true;
			}
			void FreeUpdates()
			{
				lockUpdates = false;
			}
			bool UpdatesLocked()
			{
				return lockUpdates;
			}
			float GetX()
			{
				return x;
			}

			float GetY()
			{
				return y;
			}

			virtual float GetWidth()
			{
				return 0.0f;
			}

			virtual float GetHeight()
			{
				return 0.0f;
			}
			virtual void UpdateSelf() {}
			void Update()
			{
				if (!lockUpdates)
					UpdateSelf();
			}
			void AddEventHandler(EventHandler* evh)
			{
				this->eventHandlers.push_back(evh);
			}
			void CallMouseClicked(BaseComponent* ctrl)
			{
				if (!lockHandlers)
					for (EventHandler* handler : eventHandlers)
					{
						handler->MouseClicked(pge, ctrl, MOUSE_POINT);
						handler->AnyEvent(pge, ctrl);
					}
			}
			void CallMouseReleased(BaseComponent* ctrl)
			{
				if (!lockHandlers)
					for (EventHandler* handler : eventHandlers)
					{
						handler->MouseReleased(pge, ctrl, MOUSE_POINT);
						handler->AnyEvent(pge, ctrl);
					}
			}
			void CallMouseHover(BaseComponent* ctrl)
			{
				if (!lockHandlers)
					for (EventHandler* handler : eventHandlers)
					{
						handler->MouseHover(pge, ctrl, MOUSE_POINT);
						handler->AnyEvent(pge, ctrl);
					}
			}
			void CallValueChanged(BaseComponent* ctrl, float newValue)
			{
				if (!lockHandlers)
					for (EventHandler* handler : eventHandlers)
					{
						handler->ValueChanged(pge, ctrl, newValue);
						handler->AnyEvent(pge, ctrl);
					}
			}
		};

		class Box : public BaseComponent
		{
		public:
			float width;
			float height;
			bool draw;
			olc::Pixel backgroundColor;
			Box(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f, bool draw = false, olc::Pixel backgroundColor = olc::Pixel(225, 225, 225))
			{
				this->x = x;
				this->y = y;
				this->width = width;
				this->height = height;
				this->backgroundColor = backgroundColor;
			}
			float GetWidth() override { return width; }
			float GetHeight() override { return height; }
			bool Contains(float pointX, float pointY)
			{
				if ((int)pointX > (int)x && (int)pointX <= (int)x + (int)width && (int)pointY >= (int)y && (int)pointY <= (int)y + (int)height)
				{
					return true;
				}
				return false;
			}
			void UpdateSelf() override
			{
				pge->FillRect(x, y, width, height, backgroundColor);
				if (Contains(pge->GetMouseX(), pge->GetMouseY()))
				{
					CallMouseHover(this);
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						CallMouseClicked(this);
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						CallMouseReleased(this);
				}
			}
		};

		class Button : public BaseComponent
		{
		public:
			float width;
			float height;
			float textXOffset;
			float textYOffset;
			olc::Pixel backgroundColor;
			olc::Pixel backgroundColorHover;
			olc::Pixel foregroundColor;
			olc::Pixel foregroundColorHover;
			std::string text;
			int32_t textScale;
			Button(olc::vf2d position = olc::vf2d(0.0f, 0.0f), olc::vf2d size = olc::vf2d(0.0f, 0.0f), std::string text = "Button", int32_t textScale = 2U, olc::Pixel backgroundColor = olc::Pixel(67, 112, 181), olc::Pixel backgroundColorHover = olc::Pixel(19, 52, 105), olc::Pixel foregroundColor = olc::Pixel(236, 240, 241), olc::Pixel foregroundColorHover = olc::Pixel(236, 240, 241))
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
				this->textScale = textScale;
			}
			void PlaceText(float tpx, float tpy)
			{
				textXOffset = tpx;
				textYOffset = tpy;
			}
			float GetWidth() override
			{
				return width;
			}
			float GetHeight() override
			{
				return height;
			}
			float GetTextXOffset()
			{
				return this->textXOffset;
			}
			void UpdateSelf() override
			{
				olc::Pixel backgroundColor = this->backgroundColor;
				olc::Pixel foregroundColor = this->foregroundColor;

				Box* boundingBox = new Box(x, y, width, height);

				if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY())) {
					CallMouseHover(this);
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						CallMouseClicked(this);
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						CallMouseReleased(this);

					backgroundColor = backgroundColorHover;
					foregroundColor = foregroundColorHover;
				}
				pge->FillRect(x, y, width, height, backgroundColor);
				pge->DrawString(x + textXOffset, y + textYOffset, text, foregroundColor, textScale);
				delete boundingBox;
			}
		};

		class ProgressBar : public BaseComponent
		{
		public:
			float width;
			float height;
			float value;
			float targetValue = 0.0f;
			olc::Pixel backgroundColor;
			olc::Pixel foregroundColor;
			Orientation orientation;
			ProgressBar(olc::vf2d position = olc::vf2d(0.0f, 0.0f), olc::vf2d size = olc::vf2d(0.0f, 0.0f), Orientation orientation = Orientation::AUTO, olc::Pixel backgroundColor = olc::Pixel(88, 92, 92), olc::Pixel foregroundColor = olc::Pixel(67, 112, 181))
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
			float GetWidth() override
			{
				return width;
			}
			float GetHeight() override
			{
				return height;
			}
			/* Gets the progress size of horizontal progressbar */
			float GetProgressWidth()
			{
				return (width / 100) * value;
			}
			/* Gets the progress size of vertical progressbar */
			float GetProgressHeight()
			{
				return (width / 100) * value;
			}
			Orientation GetOrientation()
			{
				return orientation;
			}
			void Increment(float incrementValue)
			{
				targetValue = value + incrementValue;
				CallValueChanged(this, value);
			}
			/* Only Increments if value in range */
			void SafeIncrement(float incrementValue)
			{
				if (!IsFull())
					Increment(incrementValue);
			}
			void Decrement(float decrementValue) {
				targetValue = value - decrementValue;
				CallValueChanged(this, value);
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
					CallValueChanged(this, value);
				}
				targetValue = newValue;
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
			void UpdateSelf() override
			{
				Box* boundingBox = new Box(x, y, width, height);
				if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY()))
				{
					CallMouseHover(this);
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						CallMouseClicked(this);
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						CallMouseReleased(this);
				}

				// Smooth progress bars
				if ((int)value != (int)targetValue)
				{
					if ((int)targetValue < (int)value)
					{
						if (!IsEmpty())
							value -= 0.4f;
					}
					else if ((int)targetValue > (int)value)
					{
						if (!IsFull())
							value += 0.4f;
					}
				}
				float newWidth = width;
				float newHeight = height;
				switch (orientation)
				{
				case HORIZONTAL:
					newWidth = (width / 100) * value;
					break;
				case VERTICAL:
					newHeight = (height / 100) * value;
					break;
				}
				pge->FillRect(x, y, width, height, backgroundColor);
				pge->FillRect(x, y, newWidth, newHeight, foregroundColor);
				delete boundingBox;
			}
		};

		class Slider : public BaseComponent
		{
		public:
			float size;
			float headOffset;
			olc::Pixel backgroundColor;
			olc::Pixel foregroundColor;
			Orientation orientation;
			bool IsSelected = false;
			Slider(olc::vf2d position = olc::vf2d(0.0f, 0.0f), float size = 0.0f, Orientation orientation = AUTO, olc::Pixel backgroundColor = olc::Pixel(88, 92, 92), olc::Pixel foregroundColor = olc::Pixel(67, 112, 181))
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
			float GetWidth() override
			{
				return size;
			}
			float GetHeight() override
			{
				return size;
			}
			void SetHeadOffset(float hoffset)
			{
				headOffset = hoffset;
			}
			float GetHeadOffset()
			{
				return headOffset;
			}
			float GetPercent()
			{
				return headOffset / (size / 100);
			}
			float Value(float maxv)
			{
				return (maxv / 100) * GetPercent();
			}
			void UpdateSelf() override
			{
				if (orientation == HORIZONTAL)
				{
					Box* boundingBox = new Box(x + headOffset, y - 30 / 2, 10, 30);
					if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY()))
					{
						CallMouseHover(this);
						if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						{
							IsSelected = true;
							CallMouseClicked(this);
						}
						if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						{
							CallMouseReleased(this);
						}
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						IsSelected = false;
					}
					if (IsSelected)
					{
						int newOffset = pge->GetMouseX() - x;
						if (newOffset >= -1 && newOffset <= (int)size + 1)
						{
							headOffset = newOffset;
							CallValueChanged(this, headOffset);
						}
					}
					pge->FillRect(x, y, size, 5, backgroundColor);
					pge->FillRect(x + headOffset, y - 30 / 2, 10, 30, foregroundColor);
					pge->FillRect(x, y, headOffset, 5, foregroundColor);
					delete boundingBox;
					return;
				}

				if (orientation == VERTICAL)
				{
					Box* boundingBox = new Box(x - 30 / 2, y + headOffset, 30, 10);
					if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY()))
					{
						CallMouseHover(this);
						if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
						{
							IsSelected = true;
							CallMouseClicked(this);
						}
						if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
						{
							CallMouseReleased(this);
						}
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						IsSelected = false;
					}
					if (IsSelected)
					{
						int newOffset = pge->GetMouseY() - y;
						if (newOffset >= -1 && newOffset <= (int)size + 1)
						{
							headOffset = newOffset;
							CallValueChanged(this, headOffset);
						}
					}
					pge->FillRect(x, y, 5, size, backgroundColor);
					pge->FillRect(x - 30 / 2, y + headOffset, 30, 10, foregroundColor);
					delete boundingBox;
					return;
				}
				return;
			}
		};

		class CheckBox : public BaseComponent
		{
		public:
			float textXOffset = 0;
			float textYOffset = 0;
			wint_t textScale;
			std::string text;
			olc::Pixel outlineColor;
			olc::Pixel selectionColor;
			olc::Pixel textColor;
			bool Checked = false;
			CheckBox(olc::vf2d position = olc::vf2d(0, 0), std::string text = "CheckBox", wint_t textScale = 2, olc::Pixel outlineColor = olc::Pixel(236, 240, 241), olc::Pixel selectionColor = olc::Pixel(231, 76, 60), olc::Pixel textColor = olc::Pixel(236, 240, 241))
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
			bool IsChecked() {
				return Checked;
			}
			void Toggle()
			{
				Checked = !Checked;
			}
			void UpdateSelf() override
			{
				Box* boundingBox = new Box(x, y, 20, 20);
				if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY())) {
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
						CallMouseClicked(this);
						CallValueChanged(this, newValue);
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased) {
						CallMouseReleased(this);
					}
				}

				if (Checked)
					pge->DrawString(x + 3, y + 3, "X", selectionColor, 2);
				pge->DrawRect(x, y, 20, 20, outlineColor);
				pge->DrawString(x + textXOffset, y + textYOffset, text, textColor, textScale);
				delete boundingBox;
			}
		};

		/* Similar to a slider but in a circle */
		class Wheel : public BaseComponent
		{
		public:
			olc::Pixel outlineColor;
			olc::Pixel dotColor;
			olc::Pixel backgroundColor;
			int width;
			int height;
			int dotSize;
			float angle = 0.0f;
			float radius = 20;
			int32_t lastValue = 0;
			Wheel(olc::vf2d position = olc::vf2d(0, 0), float radius = 0.0f, float dotSize = 0.0f, olc::Pixel outlineColor = olc::Pixel(236, 240, 241), olc::Pixel dotColor = olc::Pixel(67, 112, 181), olc::Pixel bgColor = olc::Pixel(0, 0, 0))
			{
				this->outlineColor = outlineColor;
				this->dotColor = dotColor;
				this->backgroundColor = backgroundColor;
				this->radius = radius;
				this->x = position.x;
				this->y = position.y;
				this->dotSize = dotSize;
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
			void SetAngle(float angle)
			{
				angle = angle;
			}
			const float MaxAngle()
			{
				return 133.0f;
			}
			void UpdateSelf() override
			{
				Box* boundingBox = new Box(x - radius, y - radius, radius * 2, radius * 2);
				if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY()))
				{
					CallMouseHover(this);
					if (lastValue != pge->GetMouseWheel())
					{
						CallValueChanged(this, GetPercent());
					}
					lastValue = pge->GetMouseWheel();
					if (pge->GetMouse(0).bPressed || pge->GetMouse(1).bPressed || pge->GetMouse(2).bPressed)
					{
						CallMouseClicked(this);
					}
					if (pge->GetMouse(0).bReleased || pge->GetMouse(1).bReleased || pge->GetMouse(2).bReleased)
					{
						CallMouseReleased(this);
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
		Stuff that stores a component pointer and a offset pos 
		Not really a controller though ...
		*/
		class ComponentPositionController
		{
		public:
			BaseComponent* component;
			olc::vf2d offsetPosition;
			ComponentPositionController(BaseComponent* component, olc::vf2d offsetPosition)
			{
				this->component = component;
				this->offsetPosition = offsetPosition;
			}
		};

		class Frame : public BaseComponent
		{
		public:
			olc::vf2d size;
			olc::Pixel backgroundColor;
			olc::Pixel topBarColor;
			bool selected = false;
			float topBarHeight = 15.0f;
			std::vector<ComponentPositionController> components;
			Frame(olc::vf2d position = olc::vf2d(0.0f, 0.0f), olc::vf2d size = olc::vf2d(0.0f, 0.0f), olc::Pixel backgroundColor = olc::Pixel(225, 225, 225), olc::Pixel topBarColor = olc::Pixel(67, 112, 181))
			{
				this->x = position.x;
				this->y = position.y;
				this->size = size;
				this->backgroundColor = backgroundColor;
				this->topBarColor = topBarColor;
			}
			void AddBaseComponent(BaseComponent* basecomp, olc::vf2d offsetPosition = olc::vf2d(0.0f, 0.0f))
			{
				this->components.push_back(ComponentPositionController(basecomp, offsetPosition));
			}
			void UpdateContents()
			{
				for (int i = 0; i < components.size(); i ++)
				{
					ComponentPositionController cpc = components[i];
					BaseComponent* bcomp = cpc.component;
					bcomp->x = x + cpc.offsetPosition.x;
					bcomp->y = y + topBarHeight + cpc.offsetPosition.y;
					cpc.component = bcomp;
					components[i] = cpc;
				}
			}
			void UpdateSelf() override
			{
				Box* boundingBox = new Box(x, y, size.x, topBarHeight);
				if (pge->GetMouse(0).bPressed)
				{
					if (boundingBox->Contains(pge->GetMouseX(), pge->GetMouseY()))
						selected = true;
					else
						selected = false;
				}
				if (pge->GetMouse(0).bReleased)
					selected = false;
				if (selected)
				{
					x = pge->GetMouseX();
					y = pge->GetMouseY();
				}
				pge->FillRect(x, y, size.x, size.y, backgroundColor);
				pge->FillRect(x, y, size.x, topBarHeight, topBarColor);
				delete boundingBox;
			}

		};

		/* A basic checkbox pool */
		class CheckBoxPool : public EventHandler
		{
		public:
			std::vector<CheckBox*> checkboxVector;
			CheckBoxPool(std::vector<CheckBox*> checkboxVector = {})
			{
				this->checkboxVector = checkboxVector;
			}
			void AddItem(CheckBox* checkbox)
			{
				this->checkboxVector.push_back(checkbox);
			}
			void SetItems(std::vector<CheckBox*> checkboxVector)
			{
				this->checkboxVector = checkboxVector;
			}
			void ValueChanged(olc::PixelGameEngine* pge, BaseComponent* ctrl, float nVal) override
			{
				for (CheckBox* cb : checkboxVector)
				{
					cb->Checked = false;
				}
				((CheckBox*)ctrl)->Checked = true;
			}
		};

		/* The easiest way of setting up a CPool_Handler */
		CheckBoxPool* SetupPool(std::vector<CheckBox*> checkboxVector)
		{
			CheckBoxPool* checkBoxPool = new CheckBoxPool();
			checkBoxPool->checkboxVector = checkboxVector;
			for (CheckBox* cb : checkboxVector)
			{
				cb->AddEventHandler(checkBoxPool);
			}
			return checkBoxPool;
		}

	}
}
#endif
