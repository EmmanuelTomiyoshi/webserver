#!/usr/bin/perl

use strict;
use warnings;
use CGI;
use File::Basename;

# Set the path to store uploaded files
my $upload_dir = "./front/uploads";

# print STDERR "STDIN\n\n";
# while (<STDIN>) {
#     print STDERR $_;
# }

print STDERR "\nEXECUTING CGI...\n";
# Create a CGI object
my $cgi = CGI->new;

while (1)
{
    
}

# Get the uploaded file handle
my $filehandle = $cgi->upload('image');

# Check if a file was uploaded
if ($filehandle) {
    # Get the filename
    my $filename = $cgi->param('image');
    my $basename = basename($filename);

    # Open a new file for writing in the upload directory
    open(my $outfile, '>', "$upload_dir/$basename") or die "Unable to open file: $!";

    # Copy the uploaded file to the new file
    while (my $bytesread = read($filehandle, my $buffer, 1024)) {
        print $outfile $buffer;
    }

    # Close the filehandle
    close($outfile);

    # Print success message
    print $cgi->header,
          $cgi->start_html('File Upload'),
          $cgi->h1('File Uploaded Successfully'),
          $cgi->p("Uploaded file: $basename"),
          $cgi->end_html;
} else {
    # Print error message if no file was uploaded
    print $cgi->header,
          $cgi->start_html('File Upload'),
          $cgi->h1('Error'),
          $cgi->p('No file was uploaded.'),
          $cgi->end_html;
}
