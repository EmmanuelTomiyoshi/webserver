#!/usr/bin/perl

use strict;
use warnings;
use CGI;
use CGI::Carp qw(fatalsToBrowser);
use File::Basename;

# Create a new CGI object
my $cgi = CGI->new;

# Retrieve form data (additional fields if necessary)
my $name = $cgi->param('name');
my $age = $cgi->param('age');

# Retrieve the uploaded file
my $filename = $cgi->param('image');
my $upload_filehandle = $cgi->upload('image');

# Directory to save the uploaded file
my $upload_dir = "./front/uploads";

# Ensure the upload directory exists
unless (-d $upload_dir) {
    mkdir $upload_dir or die "Cannot create upload directory: $!";
}

# Save the uploaded file
if ($upload_filehandle) {
    my $basename = basename($filename);
    my $filepath = "$upload_dir/$basename";

    open(my $fh, '>', $filepath) or die "Cannot open file $filepath: $!";
    binmode $fh; # Ensure binary mode

    while (my $chunk = <$upload_filehandle>) {
        print $fh $chunk;
    }

    close $fh;

    # Verify file was saved correctly (Optional step)
    unless (-e $filepath) {
        die "File upload failed: $filepath does not exist";
    }
}

# Send the HTTP header and start the HTML response
print $cgi->header('text/html');
print $cgi->start_html('Form Submission');

# Print the received form data
print $cgi->h1('Form Data Received');
print $cgi->p("Name: $name");
print $cgi->p("Age: $age");

# Print the file upload result
if ($upload_filehandle) {
    print $cgi->p("File uploaded successfully: $filename");
} else {
    print $cgi->p("No file uploaded.");
}

# End the HTML response
print $cgi->end_html;
