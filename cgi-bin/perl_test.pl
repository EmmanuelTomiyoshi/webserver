#!/usr/bin/perl

use strict;
use warnings;
use CGI;
# use CGI::Carp qw(fatalsToBrowser);
use File::Basename;
use Data::Dumper;

# Directory where the uploaded files will be stored
my $upload_dir = "./front/uploads";  # Change this to your desired upload directory

# Ensure the upload directory exists and is writable
unless (-d $upload_dir && -w $upload_dir) {
    die "Upload directory does not exist or is not writable";
}

print "perl envs---\n";
foreach my $key (sort keys %ENV) {
    print "$key: $ENV{$key}\n";
}
print "\n--perl envs---\n";



# Create a new CGI object
print STDERR "perl here ******\n";
sleep(1);
my $query = CGI->new;
print STDERR "perl here2 ******\n";

# Enable detailed error reporting
$CGI::POST_MAX = 1024 * 10000; # Maximum file size 10MB
$CGI::DISABLE_UPLOADS = 0; # Enable file uploads

# Set the content type for the response
print $query->header('text/html');

# Start HTML output
print <<'HTML';
<html>
<head><title>File Upload</title></head>
<body>
HTML

# Debug: Print environment variables (for debugging purposes)
print "<h3>Environment Variables</h3><pre>";
foreach my $key (sort keys %ENV) {
    print STDERR "$key = $ENV{$key}\n";
}
print "</pre>";

# Debug: Print the raw POST data (for debugging purposes)
print "<h3>Raw POST Data</h3><pre>";
my $content_length = $ENV{'CONTENT_LENGTH'};
my $raw_post_data;
read(STDIN, $raw_post_data, $content_length);
print STDERR Dumper($raw_post_data);
print "</pre>";

# Check if the form has been submitted
if ($query->request_method eq 'POST') {
    my $filename = $query->param('image');
    
    if ($filename) {
        # Get the file handle
        my $upload_filehandle = $query->upload('image');
        
        if (defined $upload_filehandle) {
            # Get the file's base name to avoid directory traversal issues
            my $basename = basename($filename);
            
            # Create the full path for the target file
            my $target = "$upload_dir/$basename";
            
            # Open the target file for writing
            open(my $out, '>', $target) or die "Could not open '$target' for writing: $!";
            
            # Copy the file contents to the target file
            while (my $bytesread = read($upload_filehandle, my $buffer, 1024)) {
                print $out $buffer;
            }
            
            close($out) or die "Could not close '$target': $!";
            
            print "<p>File '$basename' uploaded successfully!</p>";
        } else {
            print "<p>Failed to upload file: Could not get file handle.</p>";
        }
    } else {
        print "<p>No file uploaded.</p>";
    }
} else {
    # If not a POST request, show the upload form
    print <<'HTML';
    <form action="perl_test.pl" method="post" enctype="multipart/form-data">
        <p>Select an image to upload: <input type="file" name="image" /></p>
        <p><input type="submit" value="Upload" /></p>
    </form>
HTML
}

# End HTML output
print <<'HTML';
</body>
</html>
HTML

exit;
