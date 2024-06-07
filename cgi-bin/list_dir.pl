#!/usr/bin/perl
use strict;
use warnings;
use CGI qw(:standard);
use File::Basename;
use File::Spec::Functions qw(catfile);

# Set the directory to list
my $directory = param('dir') || '.';
$directory = "." if $directory eq '';

# Open the directory
opendir(my $dh, $directory) or die "Cannot open directory $directory: $!";

# Read the directory contents
my @files = readdir($dh);
closedir($dh);

# HTML header
print header;
print start_html("Index of $directory");
print h1("Index of $directory");

# Generate the table
print "<table border='1'>";
print "<tr><th>Name</th><th>Last Modified</th><th>Size</th></tr>";

foreach my $file (sort @files) {
    next if $file =~ /^\.\.?$/;  # Skip . and ..

    my $full_path = catfile($directory, $file);
    my $mod_time = (stat($full_path))[9];
    my $size = -s $full_path;

    print "<tr>";
    print "<td><a href='$directory/$file'>$file</a></td>";
    print "<td>", scalar localtime $mod_time, "</td>";
    print "<td>", defined $size ? $size : '', "</td>";
    print "</tr>";
}

print "</table>";

# HTML footer
print end_html;
