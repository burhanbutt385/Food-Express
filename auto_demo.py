import os
import time
import subprocess
import sys

# Auto-install pyautogui if it's missing
try:
    import pyautogui
except ImportError:
    print("Installing required package 'pyautogui'...")
    subprocess.check_call([sys.executable, "-m", "pip", "install", "pyautogui"])
    import pyautogui

print("=========================================================")
print("  FOODEXPRESS COMPREHENSIVE AUTO-DEMO SCRIPT")
print("=========================================================")
print("Please DO NOT touch your mouse or keyboard once the terminal opens!")
print("Starting in 3 seconds...")
time.sleep(3)

if not os.path.exists("food_express.exe"):
    print("ERROR: food_express.exe not found! Please compile the project first.")
    sys.exit(1)

os.system("start cmd /k food_express.exe")

# Wait for the new terminal window to open and become the active window
time.sleep(2.5)

def type_and_enter(text, delay_after=1.5):
    """Simulates a human typing out the text and pressing enter"""
    pyautogui.write(text, interval=0.1)
    pyautogui.press('enter')
    time.sleep(delay_after)

def press_enter(delay_after=2.0):
    """Simulates pressing enter to continue"""
    pyautogui.press('enter')
    time.sleep(delay_after)

try:
    # ---------------------------------------------------------
    # 1. DYNAMIC ORDER SCHEDULING
    # ---------------------------------------------------------
    type_and_enter("1", 1.5)
    type_and_enter("1", 1) # Insert New Order
    type_and_enter("O100", 0.3); type_and_enter("C01", 0.3)
    type_and_enter("R01", 0.3); type_and_enter("15", 0.3); type_and_enter("30", 0.3)
    press_enter(2)
    
    type_and_enter("3", 1) # Update Priority Score
    type_and_enter("O100", 0.5)
    type_and_enter("99", 0.5)
    press_enter(2)

    type_and_enter("5", 1) # Retrieve Next Processable Order
    press_enter(3)
    type_and_enter("0", 1) # Go Back

    # ---------------------------------------------------------
    # 2. KITCHEN LOAD ANALYSIS
    # ---------------------------------------------------------
    type_and_enter("2", 1.5)
    type_and_enter("1", 1) # View Workload Report
    press_enter(3)
    type_and_enter("3", 1) # Estimate Wait Time
    type_and_enter("O100", 0.5)
    press_enter(2)
    type_and_enter("0", 1) # Go Back

    # ---------------------------------------------------------
    # 3. RIDER DISPATCH OPTIMIZATION
    # ---------------------------------------------------------
    type_and_enter("3", 1.5)
    type_and_enter("1", 1) # View Rider Dashboard
    press_enter(3)
    type_and_enter("2", 1) # Auto Dispatch Rider
    press_enter(2)
    type_and_enter("4", 1) # Toggle Rider Availability
    type_and_enter("RD01", 0.5)
    press_enter(2)
    type_and_enter("0", 1) # Go Back

    # ---------------------------------------------------------
    # 4. ROUTE OPTIMIZATION (DIJKSTRA)
    # ---------------------------------------------------------
    type_and_enter("4", 1.5)
    type_and_enter("1", 1) # Shortest Path
    type_and_enter("Downtown", 0.5); type_and_enter("DHA", 0.5)
    press_enter(3)
    type_and_enter("2", 1) # Block Road
    type_and_enter("Downtown", 0.5); type_and_enter("Gulberg", 0.5)
    press_enter(2)
    type_and_enter("0", 1) # Go Back

    # ---------------------------------------------------------
    # 5. SEARCH & RETRIEVAL ENGINE
    # ---------------------------------------------------------
    type_and_enter("5", 1.5)
    type_and_enter("1", 1) # Lookup Order
    type_and_enter("O100", 0.5)
    press_enter(3)
    type_and_enter("4", 1) # Show Available Riders
    press_enter(3)
    type_and_enter("0", 1) # Go Back

    # ---------------------------------------------------------
    # 6. ORDER HISTORY TRACKING
    # ---------------------------------------------------------
    type_and_enter("6", 1.5)
    type_and_enter("O100", 1) # Select Order
    type_and_enter("1", 1) # Replay Timeline
    press_enter(3)
    type_and_enter("0", 1) # Go Back

    # ---------------------------------------------------------
    # 7. PERFORMANCE ANALYSIS
    # ---------------------------------------------------------
    type_and_enter("7", 1.5)
    type_and_enter("2", 1) # View Theoretical Complexity Report
    press_enter(4)
    type_and_enter("0", 1) # Go Back

    # ---------------------------------------------------------
    # 10. ENTITY MANAGEMENT
    # ---------------------------------------------------------
    type_and_enter("10", 1.5)
    type_and_enter("1", 1) # Register Restaurant
    type_and_enter("R99", 0.5); type_and_enter("Pizza Palace", 0.5); 
    type_and_enter("Downtown", 0.5); type_and_enter("15", 0.5)
    press_enter(2)
    type_and_enter("0", 1) # Go Back

    # ---------------------------------------------------------
    # 8. BATCH SCALABILITY SIMULATION
    # ---------------------------------------------------------
    type_and_enter("8", 5) 
    press_enter(2) 

    # ---------------------------------------------------------
    # 9. SYSTEM REPORTS
    # ---------------------------------------------------------
    type_and_enter("9", 4) 
    press_enter(2)

    # ---------------------------------------------------------
    # 11. EXIT AND SAVE
    # ---------------------------------------------------------
    type_and_enter("11", 1) 
    print("Demo Sequence Completed Successfully!")

except KeyboardInterrupt:
    print("\nDemo interrupted by user.")
