const data = [
	{
	  "title": "Continue (100)",
	  "message": "Please wait, we're processing your request.",
	  "imageSrc": "./public/images/100.png"
	},
	{
	  "title": "Switching Protocols (101)",
	  "message": "Switching to a different protocol for better performance.",
	  "imageSrc": "./public/images/101.png"
	},
	{
	  "title": "Processing (102)",
	  "message": "Your request is being processed, please hold on.",
	  "imageSrc": "./public/images/102.png"
	},
	{
	  "title": "Early Hints (103)",
	  "message": "Hints about the response are available, check back shortly.",
	  "imageSrc": "./public/images/103.png"
	},
	{
	  "title": "OK (200)",
	  "message": "Everything looks good, your request was successful.",
	  "imageSrc": "./public/images/200.png"
	},
	{
	  "title": "Created (201)",
	  "message": "Your request has been successfully created.",
	  "imageSrc": "./public/images/201.png"
	},
	{
	  "title": "Accepted (202)",
	  "message": "Your request has been accepted for processing.",
	  "imageSrc": "./public/images/202.png"
	},
	{
	  "title": "Non-Authoritative Information (203)",
	  "message": "Information provided may not be authoritative.",
	  "imageSrc": "./public/images/203.png"
	},
	{
	  "title": "No Content (204)",
	  "message": "There is no content to display at the moment.",
	  "imageSrc": "./public/images/204.png"
	},
	{
	  "title": "Reset Content (205)",
	  "message": "Content has been reset successfully.",
	  "imageSrc": "./public/images/205.png"
	},
	{
	  "title": "Partial Content (206)",
	  "message": "Only part of the content is being sent.",
	  "imageSrc": "./public/images/206.png"
	},
	{
	  "title": "Multi-Status (207)",
	  "message": "Multiple status information is available.",
	  "imageSrc": "./public/images/207.png"
	},
	{
	  "title": "Already Reported (208)",
	  "message": "The requested information has already been reported.",
	  "imageSrc": "./public/images/208.png"
	},
	{
	  "title": "IM Used (226)",
	  "message": "The server is using a different response format (Internet Messaging).",
	  "imageSrc": "./public/images/226.png"
	},
	{
	  "title": "Multiple Choices (300)",
	  "message": "Multiple options are available, please select one.",
	  "imageSrc": "./public/images/300.png"
	},
	{
	  "title": "Moved Permanently (301)",
	  "message": "The resource has been permanently moved to a different location.",
	  "imageSrc": "./public/images/301.png"
	},
	{
	  "title": "Found (302)",
	  "message": "The resource has been found, but temporarily located elsewhere.",
	  "imageSrc": "./public/images/302.png"
	},
	{
	  "title": "See Other (303)",
	  "message": "See another resource for the requested information.",
	  "imageSrc": "./public/images/303.png"
	},
	{
	  "title": "Not Modified (304)",
	  "message": "The content has not been modified since your last request.",
	  "imageSrc": "./public/images/304.png"
	},
	{
	  "title": "Use Proxy (305)",
	  "message": "Use a proxy server to access the requested resource.",
	  "imageSrc": "./public/images/305.png"
	},
	{
	  "title": "Temporary Redirect (307)",
	  "message": "This page has temporarily moved, please follow the redirect.",
	  "imageSrc": "./public/images/307.png"
	},
	{
	  "title": "Permanent Redirect (308)",
	  "message": "This page has permanently moved, please update your bookmarks.",
	  "imageSrc": "./public/images/308.png"
	},
	{
		"title": "Bad Request (400)",
		"message": "The server could not understand your request.",
		"imageSrc": "./public/images/400.png"
	},
	{
		"title": "Unauthorized (401)",
		"message": "You are not authorized to access this resource.",
		"imageSrc": "./public/images/401.png"
	},
	{
		"title": "Payment Required (402)",
		"message": "Payment is required to access this resource.",
		"imageSrc": "./public/images/402.png"
	},
	{
		"title": "Forbidden (403)",
		"message": "Access to this resource is forbidden.",
		"imageSrc": "./public/images/403.png"
	},
	{
		"title": "Not Found (404)",
		"message": "Oops! We couldn't find the page you were looking for.",
		"imageSrc": "./public/images/404.png"
	},
	{
		"title": "Method Not Allowed (405)",
		"message": "The requested method is not allowed for this resource.",
		"imageSrc": "./public/images/405.png"
	},
	{
		"title": "Not Acceptable (406)",
		"message": "The server cannot generate content acceptable to the client.",
		"imageSrc": "./public/images/406.png"
	},
	{
		"title": "Proxy Authentication Required (407)",
		"message": "Proxy authentication is required to access this resource.",
		"imageSrc": "./public/images/407.png"
	},
	{
		"title": "Request Timeout (408)",
		"message": "The server timed out waiting for the request.",
		"imageSrc": "./public/images/408.png"
	},
	{
		"title": "Conflict (409)",
		"message": "There was a conflict with your request.",
		"imageSrc": "./public/images/409.png"
	},
	{
		"title": "Gone (410)",
		"message": "The requested resource is no longer available.",
		"imageSrc": "./public/images/410.png"
	},
	{
		"title": "Length Required (411)",
		"message": "Content length is required for this request.",
		"imageSrc": "./public/images/411.png"
	},
	{
		"title": "Precondition Failed (412)",
		"message": "Precondition for the request failed.",
		"imageSrc": "./public/images/412.png"
	},
	{
		"title": "Payload Too Large (413)",
		"message": "The request payload is too large.",
		"imageSrc": "./public/images/413.png"
	},
	{
		"title": "URI Too Long (414)",
		"message": "The URI provided is too long.",
		"imageSrc": "./public/images/414.png"
	},
	{
		"title": "Unsupported Media Type (415)",
		"message": "The media type of the request is not supported.",
		"imageSrc": "./public/images/415.png"
	},
	{
		"title": "Range Not Satisfiable (416)",
		"message": "The requested range is not satisfiable.",
		"imageSrc": "./public/images/416.png"
	},
	{
		"title": "Expectation Failed (417)",
		"message": "The server cannot meet the requirements of the Expect request-header field.",
		"imageSrc": "./public/images/417.png"
	},
	{
		"title": "I'm a teapot (418)",
		"message": "I'm a teapot. This server is not designed to brew coffee.",
		"imageSrc": "./public/images/418.png"
	},
	{
		"title": "Misdirected Request (421)",
		"message": "The request was directed at a server that is not able to produce a response.",
		"imageSrc": "./public/images/421.png"
	},
	{
		"title": "Unprocessable Entity (422)",
		"message": "The server understands the content type of the request entity but was unable to process the contained instructions.",
		"imageSrc": "./public/images/422.png"
	},
	{
		"title": "Locked (423)",
		"message": "The resource is locked and cannot be accessed.",
		"imageSrc": "./public/images/423.png"
	},
	{
		"title": "Failed Dependency (424)",
		"message": "The request failed due to a dependency on another request.",
		"imageSrc": "./public/images/424.png"
	},
	{
		"title": "Too Early (425)",
		"message": "The server is unwilling to risk processing a request that might be replayed.",
		"imageSrc": "./public/images/425.png"
	},
	{
		"title": "Upgrade Required (426)",
		"message": "Upgrade to a different protocol version to access this resource.",
		"imageSrc": "./public/images/426.png"
	},
	{
		"title": "Precondition Required (428)",
		"message": "The server requires the request to be conditional.",
		"imageSrc": "./public/images/428.png"
	},
	{
		"title": "Too Many Requests (429)",
		"message": "Slow down! You've reached the limit of requests allowed.",
		"imageSrc": "./public/images/429.png"
	},
	{
		"title": "Request Header Fields Too Large (431)",
		"message": "The request header is too large for the server to process.",
		"imageSrc": "./public/images/431.png"
	},
	{
		"title": "Unavailable For Legal Reasons (451)",
		"message": "Access to this resource is unavailable due to legal reasons.",
		"imageSrc": "./public/images/451.png"
	},
	{
		"title": "Internal Server Error (500)",
		"message": "Oops! Something went wrong on our end.",
		"imageSrc": "./public/images/500.png"
	},
	{
		"title": "Not Implemented (501)",
		"message": "The server does not support the functionality required to fulfill the request.",
		"imageSrc": "./public/images/501.png"
	},
	{
		"title": "Bad Gateway (502)",
		"message": "The server received an invalid response from the upstream server.",
		"imageSrc": "./public/images/502.png"
	},
	{
		"title": "Service Unavailable (503)",
		"message": "The service is temporarily unavailable, please try again later.",
		"imageSrc": "./public/images/503.png"
	},
	{
		"title": "Gateway Timeout (504)",
		"message": "The gateway timed out while waiting for a response.",
		"imageSrc": "./public/images/504.png"
	},
	{
		"title": "HTTP Version Not Supported (505)",
		"message": "The HTTP version used in the request is not supported.",
		"imageSrc": "./public/images/505.png"
	},
	{
		"title": "Variant Also Negotiates (506)",
		"message": "The server has an internal configuration error.",
		"imageSrc": "./public/images/506.png"
	},
	{
		"title": "Insufficient Storage (507)",
		"message": "The server is out of storage space.",
		"imageSrc": "./public/images/507.png"
	},
	{
		"title": "Loop Detected (508)",
		"message": "The server detected an infinite loop while processing the request.",
		"imageSrc": "./public/images/508.png"
	},
	{
		"title": "Not Extended (510)",
		"message": "Further extensions to the request are required for the server to fulfill it.",
		"imageSrc": "./public/images/510.png"
	},
	{
		"title": "Network Authentication Required (511)",
		"message": "Authentication is required to access the network.",
		"imageSrc": "./public/images/511.png"
	}
  ]
  

document.addEventListener('DOMContentLoaded', function () {
  
    let currentIndex = 0; // Variable to keep track of the current index

    const headingPlaceholder = document.getElementById('headingPlaceholder');
    const messagePlaceholder = document.getElementById('messagePlaceholder');
    const imageElement = document.getElementById('imagePlaceholder');

    // Function to update content based on current index
    function updateContent(index) {
      const currentObject = data[index];
      headingPlaceholder.textContent = currentObject.title;
      messagePlaceholder.textContent = currentObject.message;
      imageElement.src = currentObject.imageSrc;
    }

    // Display the first object initially
    updateContent(currentIndex);

    const button = document.querySelector('.Button');
    button.addEventListener('click', function () {
      currentIndex++; // Increment the index to display the next object
      if (currentIndex >= data.length) {
        currentIndex = 0; // Reset index to 0 if it exceeds the array length
      }
      updateContent(currentIndex);
    });

  });