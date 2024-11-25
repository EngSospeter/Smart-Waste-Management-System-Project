// Fetch and update bin level when the page loads
    fetchBinLevel();

    // Periodically update the bin level every 5 seconds (for demo purposes)
    setInterval(fetchBinLevel, 5000);
    
// Function to fetch the current bin level from the Flask backend
function fetchBinLevel() {
    console.log("Fetching current bin level...");  // Log the fetch attempt
    fetch('/current_bin_level')
        .then(response => response.json())
        .then(data => {
            console.log("Bin Level Data:", data);  // Log the data received from the backend

            if (data.level !== undefined) {
                updateBinPercentage(data.level);
            } else {
                console.error("Error: Bin level not returned");  // Log error if level is undefined
            }
        })
        .catch(error => console.error('Error fetching bin level:', error));
}

// Function to update the bin percentage based on the bin level
function updateBinPercentage(level) {
    console.log(`Updating bin percentage to: ${level}%`);  // Log the update action

    const wastePercentage = document.getElementById('waste-percentage');
    
    // Update the percentage text
    wastePercentage.textContent = `${level}%`;
}

// Function to handle scheduling collection
function scheduleCollection() {
    const date = document.getElementById('collection-date').value;
    const time = document.getElementById('time-slot').value;

    if (date && time) {
        // Send data to the server
        fetch('/schedule', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ date, time }),
        })
        .then(response => response.json())
        .then(data => {
            if (data.success) {
                alert(`Collection scheduled for ${date} at ${time}`);
            } else {
                alert("Failed to schedule collection. Try again.");
            }
        })
        .catch(error => console.error('Error:', error));
    } else {
        alert("Please select a date and time slot.");
    }
}
