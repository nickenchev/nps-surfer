#ifndef PROGRESSITEM_H
#define PROGRESSITEM_H

#include <string>

enum class ProgressState
{
	Waiting,
	Working,
	Success,
	Error
};

struct ProgressItem
{
	std::string title;
	ProgressState state;

    ProgressItem(const std::string &title);
};

#endif /* PROGRESSITEM_H */
