document.addEventListener('DOMContentLoaded', function () {
    fetch('../status_codes.json')
      .then(response => response.json())
      .then(data => {
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
      })
      .catch(error => console.error('Error fetching JSON:', error));
  });