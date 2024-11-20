from flask import Flask, request, jsonify

# Create a Flask application instance
app = Flask(__name__)

# Root route to display a welcome message
@app.route('/')
def home():
    return "Welcome to the Smart Waste Management System API!"

# Endpoint to receive bin level data
@app.route('/update-bin-level', methods=['POST'])
def update_bin_level():
    # Extract the incoming JSON data from the request
    data = request.json  # Expecting JSON payload

    # Retrieve the 'fill_level' from the data
    fill_level = data.get('fill_level')  # Gets the 'fill_level' value or None if it doesn't exist

    # Validate the received data
    if fill_level is None:  # Check if 'fill_level' is missing
        return jsonify(
            {"error": "Invalid data"}), 400  # Respond with an error message and HTTP status code 400 (Bad Request)

    # Log or process the fill level
    print(f"Received fill level: {fill_level}%")  # Print the received fill level to the console for debugging

    # Send acknowledgment
    return jsonify({
                       "message": "Fill level received successfully"}), 200  # Respond with a success message and HTTP status code 200 (OK)


# Run the Flask app only if this file is executed directly
if __name__ == '__main__':
    # Start the Flask app, making it accessible on all network interfaces (host='0.0.0.0') and listening on port 5000
    app.run(host='0.0.0.0', port=5000)
