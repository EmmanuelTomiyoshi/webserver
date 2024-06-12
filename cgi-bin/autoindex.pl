#!/usr/bin/perl
use strict;
use warnings;
use CGI qw(:standard);
use File::Basename;
use File::Spec::Functions qw(catfile);
use URI::Escape;

# Get the directory or file from the query parameter or use the current directory
my $path = param('dir') || '.';
$path =~ s{/$}{};  # Remove the trailing slash if present

# Check if the path is a directory or a file
if (-d $path) {
    # It's a directory, generate the directory listing

    # Print the HTTP header and the beginning of the HTML page
    print header;
    print start_html("Index of $path");

    # Print the directory listing
    print "<h1>Index of $path</h1>\n";
    print "<hr>\n";
    print "<ul>\n";

    # Open the directory
    opendir(my $dh, $path) or die "Cannot open directory $path: $!";

    # Read the directory contents
    my @entries = readdir($dh);

    # Sort the entries, directories first
    @entries = sort {
        my $a_is_dir = -d catfile($path, $a);
        my $b_is_dir = -d catfile($path, $b);
        return $b_is_dir <=> $a_is_dir || $a cmp $b;
    } @entries;

    foreach my $entry (@entries) {
        next if $entry eq '.' || $entry eq '..';   # Skip the current and parent directory entries
        my $full_path = catfile($path, $entry);
        my $encoded_entry = uri_escape($entry);
        my $encoded_path = uri_escape($path);
        my $url = $encoded_entry;
        $url = "$encoded_path/$encoded_entry" if $path ne '.';
        if (-d $full_path) {
            print "<li><a href=\"?dir=$url/\">[DIR] $entry</a></li>\n";
        } else {
            print "<li><a href=\"?dir=$url\">$entry</a></li>\n";
        }
    }

    # Close the directory
    closedir($dh);

    print "</ul>\n";
    print "<hr>\n";
    print end_html;
} elsif (-f $path) {
    # It's a file, serve the file contents

    # Determine the content type based on the file extension
    my $type = 'text/plain';
    if ($path =~ /\.html?$/i) {
        $type = 'text/html';
    }

    # Print the HTTP header with the correct content type
    print header(-type => $type);

    # Read and print the file contents
    open(my $fh, '<', $path) or die "Cannot open file $path: $!";
    while (my $line = <$fh>) {
        print $line;
    }
    close($fh);
} else {
    # Path does not exist
    print header;
    print start_html("Error");
    print "<h1>Error</h1>\n";
    print "<p>Path $path does not exist.</p>\n";
    print end_html;
}

__END__
