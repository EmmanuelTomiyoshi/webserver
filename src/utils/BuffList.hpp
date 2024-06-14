#ifndef BUFF_LIST_HPP
# define BUFF_LIST_HPP
# include "base.hpp"
/* 
	data_buff == {char *buff; ssize_t size; ssize_t count}

	event_data->buffer_list->front()->buff; 
	event_data->buffer_list->front()->size;
	event_data->buffer_list->front()->count;

	delete node that count == size;

	add new data to the back of the list

	read from the front of the list


	- create event handler that will add new buffer chuncks to buffer_list

	- create event handler that will send buffer chuncks from buffer_list to client
	- when all the buff in a chunck got read -> then delete the buffer and remove the chunck
 */

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