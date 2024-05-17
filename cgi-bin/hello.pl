#!/usr/bin/perl

use strict;
use warnings;
use CGI;
use CGI::Carp qw(fatalsToBrowser);

# Create a new CGI object
my $cgi = CGI->new;

# Send the HTTP header and start the HTML response
print $cgi->header('text/html');
print $cgi->start_html('Welcome Page');

# Generate the welcome message
print $cgi->h1('Welcome!');
print $cgi->p('Hello, and welcome to our website. We are glad to have you here.');

# End the HTML response
print $cgi->end_html;
