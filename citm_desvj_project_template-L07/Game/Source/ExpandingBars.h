#ifndef __EXPANDING_BARS_H__
#define __EXPANDING_BARS_H__

#include "Transition.h"
#include "Bar.h"

class ExpandingBars : public Transition
{
public:
	ExpandingBars(SceneID next_scene, float step_duration, bool non_lerp, int bar_number, bool horizontal, bool random_colours, Color even_color, Color odd_color);
	~ExpandingBars();

	void StepTransition();

public:
	void Entering();
	void Changing();
	void Exiting();

	void ExpandBars();
	void ExpandHorizontalBars();
	void ExpandVerticalBars();
	void DrawBars();

	void InitExpandingBars();
	void AssignHorizontalBar(Bar& new_bar, const int& win_width, const int& win_height, const int& index);
	void AssignVerticalBar(Bar& new_bar, const int& win_width, const int& win_height, const int& index);
	void AssignBarColour(Bar& new_bar, const int& index);

private:
	
	List<Bar> bars;

	fPoint screen_center;

	bool vertical;
	bool random_colours;

	int	bar_number;
	int	win_width;
	int	win_height;

	Color even_color;
	Color odd_color;;
};

#endif // !__EXPANDING_BARS_H__
