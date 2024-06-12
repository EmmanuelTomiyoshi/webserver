#!/usr/bin/perl
use strict;
use warnings;
use CGI;

print STDERR "executiong cgi get script\n";
# Create a new CGI object
my $cgi = CGI->new;

# Get the query parameters
my %params = $cgi->Vars;

# Set the content type for the HTTP response
print $cgi->header('text/html');

# Start the HTML response
print "<html><head><title>Perl CGI GET Request</title></head><body>";
print "<h1>GET Request Parameters</h1>";

# Check if there are any parameters
if (%params) {
    print "<ul>";
    # Loop through each parameter and print it
    foreach my $key (keys %params) {
        print "<li><strong>$key</strong>: " . $cgi->escapeHTML($params{$key}) . "</li>";
    }
    print "</ul>";
} else {
    print "<p>No parameters found.</p>";
}

# End the HTML response
print "</body></html>";
