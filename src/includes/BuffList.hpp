#ifndef BUFF_LIST_HPP
# define BUFF_LIST_HPP
# include "base.hpp"

class BuffList
{
    public:
        class BuffData
		{
			public:
				BuffData(void);
				char *buff;
				ssize_t size;
				ssize_t count;
		};
    private:
		std::list<BuffData> _buff_list;

    public:
		BuffList(void);
		~BuffList(void);
		void add(char *buff, ssize_t size);
		BuffData & front(void);
		BuffData & back(void);
		void del_front(void);
		void del_back(void);
		void clear(void);
};

#endif