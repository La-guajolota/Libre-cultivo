import streamlit as st
from menu import menu_with_redirect

# Redirect to app.py if not logged in, otherwise show the navigation menu
menu_with_redirect()

st.title("Intelligent System for Urban Hydroponic Farming")
st.markdown(f"A product broght by Libre Cultivo.")