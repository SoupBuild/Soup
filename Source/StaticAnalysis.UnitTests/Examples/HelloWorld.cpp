int main()
{
    std::cout << "Hello World!";
	char test = 's';
	auto func = [&]() mutable noexcept {return 1;};
	return 0;
}
