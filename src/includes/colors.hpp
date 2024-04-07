#ifndef COLORS_HPP
# define COLORS_HPP

//Regular text
# define COLOR_BLACK   "\001\e[0;30m\002"
# define COLOR_RED     "\001\e[0;31m\002"
# define COLOR_GREEN   "\001\e[0;32m\002"
# define COLOR_YELLOW  "\001\e[0;33m\002"
# define COLOR_BLUE    "\001\e[0;34m\002"
# define COLOR_MAGENTA "\001\e[0;35m\002"
# define COLOR_CYAN    "\001\e[0;36m\002"
# define COLOR_WHITE   "\001\e[0;37m\002"

//Regular underline text
# define COLOR_UBLACK   "\001\e[4;30m\002"
# define COLOR_URED     "\001\e[4;31m\002"
# define COLOR_UGREEN   "\001\e[4;32m\002"
# define COLOR_UYELLOW  "\001\e[4;33m\002"
# define COLOR_UBLUE    "\001\e[4;34m\002"
# define COLOR_UMAGENTA "\001\e[4;35m\002"
# define COLOR_UCYAN    "\001\e[4;36m\002"
# define COLOR_UWHITE   "\001\e[4;37m\002"

//Regular background
# define COLOR_BLACKB   "\001\e[40m\002"
# define COLOR_REDB     "\001\e[41m\002"
# define COLOR_GREENB   "\001\e[42m\002"
# define COLOR_YELLOWB  "\001\e[43m\002"
# define COLOR_BLUEB    "\001\e[44m\002"
# define COLOR_MAGENTAB "\001\e[45m\002"
# define COLOR_CYANB    "\001\e[46m\002"
# define COLOR_WHITEB   "\001\e[47m\002"

//High intensty background
# define COLOR_BLACKHB   "\001\e[0;100m\002"
# define COLOR_REDHB     "\001\e[0;101m\002"
# define COLOR_GREENHB   "\001\e[0;102m\002"
# define COLOR_YELLOWHB  "\001\e[0;103m\002"
# define COLOR_BLUEHB    "\001\e[0;104m\002"
# define COLOR_MAGENTAHB "\001\e[0;105m\002"
# define COLOR_CYANHB    "\001\e[0;106m\002"
# define COLOR_WHITEHB   "\001\e[0;107m\002"

//High intensty text
# define COLOR_HBLACK   "\001\e[0;90m\002"
# define COLOR_HRED     "\001\e[0;91m\002"
# define COLOR_HGREEN   "\001\e[0;92m\002"
# define COLOR_HYELLOW  "\001\e[0;93m\002"
# define COLOR_HBLUE    "\001\e[0;94m\002"
# define COLOR_HMAGENTA "\001\e[0;95m\002"
# define COLOR_HCYAN    "\001\e[0;96m\002"
# define COLOR_HWHITE   "\001\e[0;97m\002"

//Bold high intensity text
# define COLOR_BHBLACK   "\001\e[1;90m\002"
# define COLOR_BHRED     "\001\e[1;91m\002"
# define COLOR_BHGREEN   "\001\e[1;92m\002"
# define COLOR_BHYELLOW  "\001\e[1;93m\002"
# define COLOR_BHBLUE    "\001\e[1;94m\002"
# define COLOR_BHMAGENTA "\001\e[1;95m\002"
# define COLOR_BHCYAN    "\001\e[1;96m\002"
# define COLOR_BHWHITE   "\001\e[1;97m\002"

//Reset
# define COLOR_RESET       "\001\e[0m\002"

#endif // COLORS_HPP