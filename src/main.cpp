#include <iostream>
#include "dataloader.h"
#include "title.h"
#include <vector>

int main(int argc, char *argv[])
{
	DataLoader loader;
	const std::vector<Title> allTitles = loader.load();

	std::cout << "Read " << allTitles.size() << " titles" << std::endl;
}
