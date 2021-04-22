#include <cli.h>

void CLi::intro( void )
{
  clog( Color::blue );
  clog( "\t\t ________  ___  ___  ________  ___  ___  __    ________       " );
  clog( "\t\t|\\   __  \\|\\  \\|\\  \\|\\   __  \\|\\  \\|\\  \\|\\  \\ |\\   ____\\      " );
  clog( "\t\t\\ \\  \\|\\  \\ \\  \\\\\\  \\ \\  \\|\\ /\\ \\  \\ \\  \\/  /|\\ \\  \\___|_     " );
  clog( "\t\t \\ \\   _  _\\ \\  \\\\\\  \\ \\   __  \\ \\  \\ \\   ___  \\ \\_____  \\    " );
  clog( "\t\t  \\ \\  \\\\  \\\\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\ \\  \\\\ \\  \\|____|\\  \\   " );
  clog( "\t\t   \\ \\__\\\\ _\\\\ \\_______\\ \\_______\\ \\__\\ \\__\\\\ \\__\\____\\_\\  \\  " );
  clog( "\t\t    \\|__|\\|__|\\|_______|\\|_______|\\|__|\\|__| \\|__|\\_________\\ " );
  clog( "\t\t                                                 \\|_________| " );
  
  clog( Color::red );
  clog( "\t\t        ________  ___  ___  ________  _______                 " );
  clog( "\t\t       |\\   ____\\|\\  \\|\\  \\|\\   __  \\|\\  ___ \\                " );
  clog( "\t\t       \\ \\  \\___|\\ \\  \\\\\\  \\ \\  \\|\\ /\\ \\   __/|               " );
  clog( "\t\t        \\ \\  \\    \\ \\  \\\\\\  \\ \\   __  \\ \\  \\_|/__             " );
  clog( "\t\t         \\ \\  \\____\\ \\  \\\\\\  \\ \\  \\|\\  \\ \\  \\_|\\ \\            " );
  clog( "\t\t          \\ \\_______\\ \\_______\\ \\_______\\ \\_______\\           " );
  clog( "\t\t           \\|_______|\\|_______|\\|_______|\\|_______|           " );
  clog( "\t\t                                                     " );
  clog( Color::gray, "\t\t  This is a free software licensed under the  ", Color::yellow, " ██╗     ██╗███████╗██████╗  " );
  clog( Color::gray, "\t\t                                              ", Color::yellow, " ██║     ██║██╔════╝██╔══██╗ " );
  clog( Color::white, "\t\t       GNU GENERAL PUBLIC LICENSE             ",Color::yellow, " ██║     ██║███████╗██████╔╝ " );
  clog( Color::gray, "\t\t TO LEARN MORE ABOUT THE TERMS AND CONDITIONS ", Color::yellow, " ██║     ██║╚════██║██╔═══╝  " );
  clog( Color::gray, "\t\t PLEASE READ THE ATTACHED LICENSE FILE        ", Color::yellow, " ███████╗██║███████║██║      " );
  clog( Color::gray, "\t\t CONTAINED IN THE SOURCE FOLDER               ", Color::yellow, " ╚══════╝╚═╝╚══════╝╚═╝      " );
  slog_( "", Color::white, "\t\t                                                             (v ", CURRENT_VERSION_NUMBERS[0], '.', CURRENT_VERSION_NUMBERS[1], '.', CURRENT_VERSION_NUMBERS[2], ')' );
  NL( Color::off );
}
