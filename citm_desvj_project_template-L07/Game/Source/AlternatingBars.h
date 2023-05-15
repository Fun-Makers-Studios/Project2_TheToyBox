#ifndef __ALTERNATING_BARS_H__
#define __ALTERNAING_BARS_H__

#include "Transition.h"
#include "Bar.h"

class AlternatingBars : public Transition
{
public:

	AlternatingBars(int bar_number, bool vertical, bool random_colours, Color even_color, Color odd_color);
	~AlternatingBars();

public:

	void DoTransition();

	void TranslateHorizontalBars();
	void TranslateVerticalBars();
	void DrawBars();
	
	void InitAlternatingBars();
	void AssignHorizontalBar(Bar& new_bar, const int& win_width, const int& win_height, const int& index);
	void AssignVerticalBar(Bar& new_bar, const int& win_width, const int& win_height, const int& index);
	void AssignBarColour(Bar& new_bar, const int& index);

private:
	
	List<Bar> bars;

	bool vertical;
	bool random_colours;
	
	int	bar_number;
	int win_width;
	int win_height;

	Color even_color;
	Color odd_color;
};

#endif // !__ALTERNATE_BARS_H__