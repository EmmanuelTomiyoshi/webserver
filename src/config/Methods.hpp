#ifndef METHODS_HPP
# define METHODS_HPP
# include <webserver.hpp>

class Methods {
	private:
		static t_vec _methods_available;
		std::map<t_str, bool> _methods;

	public:
		Methods(void);
		~Methods(void);
		void set(t_list values);
		bool allowed(t_str method) const;
};

#endif