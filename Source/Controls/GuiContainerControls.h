/***********************************************************************
Vczh Library++ 3.0
Developer: Zihan Chen(vczh)
GacUI::Control System

Interfaces:
***********************************************************************/

#ifndef VCZH_PRESENTATION_CONTROLS_GUICONTAINERCONTROLS
#define VCZH_PRESENTATION_CONTROLS_GUICONTAINERCONTROLS

#include "GuiScrollControls.h"

namespace vl
{
	namespace presentation
	{
		namespace controls
		{
/***********************************************************************
Tab Control
***********************************************************************/

			class GuiTabPageList;
			class GuiTab;

			/// <summary>Represnets a tab page control.</summary>
			class GuiTabPage : public GuiCustomControl, public AggregatableDescription<GuiTabPage>
			{
				friend class GuiTabPageList;
				friend class GuiTab;
			protected:
				GuiTab*											tab = nullptr;

				bool											IsAltAvailable()override;
			public:
				/// <summary>Create a tab page control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiTabPage(ControlTemplateType* _controlTemplate);
				~GuiTabPage();

				GuiTab*											GetOwnerTab();
			};

			class GuiTabPageList : public collections::ObservableList<GuiTabPage*>
			{
			protected:
				GuiTab*											tab;

				bool											QueryInsert(vint index, GuiTabPage* const& value)override;
				void											AfterInsert(vint index, GuiTabPage* const& value)override;
				void											BeforeRemove(vint index, GuiTabPage* const& value)override;
			public:
				GuiTabPageList(GuiTab* _tab);
				~GuiTabPageList();
			};

			/// <summary>Represents a container with multiple named tabs.</summary>
			class GuiTab : public GuiControl, public Description<GuiTab>
			{
				friend class GuiTabPage;
				friend class GuiTabPageList;
			protected:
				class CommandExecutor : public Object, public ITabCommandExecutor
				{
				protected:
					GuiTab*										tab;
				public:
					CommandExecutor(GuiTab* _tab);
					~CommandExecutor();

					void										ShowTab(vint index)override;
				};

				Ptr<CommandExecutor>							commandExecutor;
				IStyleController*								styleController = nullptr;
				GuiTabPageList									tabPages;
				GuiTabPage*										selectedPage = nullptr;
			public:
				/// <summary>Create a control with a specified style controller.</summary>
				/// <param name="_styleController">The style controller.</param>
				GuiTab(ControlTemplateType* _controlTemplate);
				~GuiTab();

				/// <summary>Selected page changed event.</summary>
				compositions::GuiNotifyEvent					SelectedPageChanged;

				/// <summary>Get all pages.</summary>
				/// <returns>All pages.</returns>
				collections::ObservableList<GuiTabPage*>&		GetPages();

				/// <summary>Get the selected page.</summary>
				/// <returns>The selected page.</returns>
				GuiTabPage*										GetSelectedPage();
				/// <summary>Set the selected page.</summary>
				/// <returns>Returns true if this operation succeeded.</returns>
				/// <param name="value">The selected page.</param>
				bool											SetSelectedPage(GuiTabPage* value);
			};

/***********************************************************************
Scroll View
***********************************************************************/

			/// <summary>A control with a vertical scroll bar and a horizontal scroll bar to perform partial viewing.</summary>
			class GuiScrollView : public GuiControl, public Description<GuiScrollView>
			{
			protected:
				class CommandExecutor : public Object, public IScrollViewCommandExecutor
				{
				protected:
					GuiScrollView*						scrollView;

				public:
					CommandExecutor(GuiScrollView* _scrollView);
					~CommandExecutor();

					void								CalculateView()override;
				};

				Ptr<CommandExecutor>					commandExecutor;
				IStyleController*						styleController;
				bool									supressScrolling;

				void									OnContainerBoundsChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									OnHorizontalScroll(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									OnVerticalScroll(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				void									OnHorizontalWheel(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void									OnVerticalWheel(compositions::GuiGraphicsComposition* sender, compositions::GuiMouseEventArgs& arguments);
				void									CallUpdateView();

				/// <summary>Calculate the full size of the content.</summary>
				/// <returns>The full size of the content.</returns>
				virtual Size							QueryFullSize()=0;
				/// <summary>Update the visible content using a view bounds. The view bounds is in the space from (0,0) to full size.</summary>
				/// <param name="viewBounds">The view bounds.</param>
				virtual void							UpdateView(Rect viewBounds)=0;
				/// <summary>Calculate the small move of the scroll bar.</summary>
				/// <returns>The small move of the scroll bar.</returns>
				virtual vint							GetSmallMove();
				/// <summary>Calculate the big move of the scroll bar.</summary>
				/// <returns>The big move of the scroll bar.</returns>
				virtual Size							GetBigMove();
				
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleController">The style controller.</param>
				GuiScrollView(ControlTemplateType* _controlTemplate);
				~GuiScrollView();

				virtual void							SetFont(const FontProperties& value);

				/// <summary>Force to update contents and scroll bars.</summary>
				void									CalculateView();
				/// <summary>Get the view size.</summary>
				/// <returns>The view size.</returns>
				Size									GetViewSize();
				/// <summary>Get the view bounds.</summary>
				/// <returns>The view bounds.</returns>
				Rect									GetViewBounds();
				
				/// <summary>Get the horizontal scroll control.</summary>
				/// <returns>The horizontal scroll control.</returns>
				GuiScroll*								GetHorizontalScroll();
				/// <summary>Get the vertical scroll control.</summary>
				/// <returns>The vertical scroll control.</returns>
				GuiScroll*								GetVerticalScroll();
				/// <summary>Test is the horizontal scroll bar always visible even the content doesn't exceed the view bounds.</summary>
				/// <returns>Returns true if the horizontal scroll bar always visible even the content doesn't exceed the view bounds</returns>
				bool									GetHorizontalAlwaysVisible();
				/// <summary>Set is the horizontal scroll bar always visible even the content doesn't exceed the view bounds.</summary>
				/// <param name="value">Set to true if the horizontal scroll bar always visible even the content doesn't exceed the view bounds</param>
				void									SetHorizontalAlwaysVisible(bool value);
				/// <summary>Test is the vertical scroll bar always visible even the content doesn't exceed the view bounds.</summary>
				/// <returns>Returns true if the vertical scroll bar always visible even the content doesn't exceed the view bounds</returns>
				bool									GetVerticalAlwaysVisible();
				/// <summary>Set is the vertical scroll bar always visible even the content doesn't exceed the view bounds.</summary>
				/// <param name="value">Set to true if the vertical scroll bar always visible even the content doesn't exceed the view bounds</param>
				void									SetVerticalAlwaysVisible(bool value);
			};
			
			/// <summary>A control container with a vertical scroll bar and a horizontal scroll bar to perform partial viewing. When controls are added, removed, moved or resized, the scroll bars will adjust automatically.</summary>
			class GuiScrollContainer : public GuiScrollView, public Description<GuiScrollContainer>
			{
			protected:
				bool									extendToFullWidth = false;
				compositions::GuiBoundsComposition*		containerComposition = nullptr;

				void									OnControlContainerBoundsChanged(compositions::GuiGraphicsComposition* sender, compositions::GuiEventArgs& arguments);
				Size									QueryFullSize()override;
				void									UpdateView(Rect viewBounds)override;
			public:
				/// <summary>Create a control with a specified style provider.</summary>
				/// <param name="styleController">The style controller.</param>
				GuiScrollContainer(ControlTemplateType* _controlTemplate);
				~GuiScrollContainer();
				
				/// <summary>Test does the content container always extend its width to fill the scroll container.</summary>
				/// <returns>Return true if the content container always extend its width to fill the scroll container.</returns>
				bool									GetExtendToFullWidth();
				/// <summary>Set does the content container always extend its width to fill the scroll container.</summary>
				/// <param name="value">Set to true if the content container always extend its width to fill the scroll container.</param>
				void									SetExtendToFullWidth(bool value);
			};
		}
	}
}

#endif