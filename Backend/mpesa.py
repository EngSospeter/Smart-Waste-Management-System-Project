
import requests
import datetime
from requests.auth import HTTPBasicAuth
from base64 import b64encode


# Get Access Token function
def get_access_token():
    consumer_key = '6tSWC8u7A2NhsSH2kcfjfqJLZ6QJNUs6xXf2SgmuG3ilvHcu'  # Replace with your consumer key
    consumer_secret = '9T9KsStueKScIXoSYvAJ7AtnwEWtxmtmcJQXmAVMAZXGr0bKCNCOg885pmYxAGGO'  # Replace with your consumer secret
    api_url = 'https://sandbox.safaricom.co.ke/oauth/v1/generate?grant_type=client_credentials'

    response = requests.get(api_url, auth=HTTPBasicAuth(consumer_key, consumer_secret))
    if response.status_code == 200:
        return response.json().get('access_token')
    else:
        print("Failed to get access token:", response.json())
        return None


# Function to generate password for STK Push
def generate_password():
    passkey = 'bfb279f9aa9bdbcf158e97dd71a467cd2e0c893059b10f78e6b72ada1ed2c919'  # Replace with the passkey provided in the sandbox
    shortcode = '174379'  # Use the sandbox shortcode
    timestamp = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
    data = f"{shortcode}{passkey}{timestamp}"
    return b64encode(data.encode()).decode('utf-8')


# Function to convert phone number to the required format
def format_phone_number(phone_number):
    # Add the country code '254' if the number starts with '0'
    if phone_number.startswith('0'):
        return '254' + phone_number[1:]  # Remove the leading '0' and add '254' country code
    return phone_number  # Return the number as is if it already starts with '254'


# Function to initiate STK Push request
def initiate_stk_push(amount, phone_number):
    access_token = get_access_token()
    if access_token is None:
        print("Error: Could not get access token")
        return

    api_url = 'https://sandbox.safaricom.co.ke/mpesa/stkpush/v1/processrequest'
    headers = {'Authorization': f'Bearer {access_token}'}

    # Format the phone number to include the country code
    formatted_phone_number = format_phone_number(phone_number)

    # Prepare the request payload
    payload = {
        "BusinessShortCode": "174379",  # Sandbox Shortcode
        "Password": generate_password(),
        "Timestamp": datetime.datetime.now().strftime('%Y%m%d%H%M%S'),
        "TransactionType": "CustomerPayBillOnline",
        "Amount": 1,  # Use the amount passed to the function
        "PartyA": formatted_phone_number,  # Use the formatted phone number
        "PartyB": "174379",  # Use the sandbox shortcode for PartyB
        "PhoneNumber": formatted_phone_number,  # Use the formatted phone number
        "CallBackURL": "https://sospeter1609.pythonanywhere.com/apiResp",  # Update this to your real callback URL
        "AccountReference": "Okota Taka",
        "TransactionDesc": "Garbage Collection Payments"
    }

    response = requests.post(api_url, json=payload, headers=headers)
    return response.json()
