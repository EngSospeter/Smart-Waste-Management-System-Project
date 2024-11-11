function scheduleCollection() {
    const date = document.getElementById('collection-date').value;
    const time = document.getElementById('time-slot').value;

    if (date && time) {
        alert(`Collection scheduled for ${date} at ${time}`);
    } else {
        alert("Please select a date and time slot.");
    }
}
