
// This file's code is LLM generated - I suck at frontend :(

// The phone sometimes disconnects the Websocket, add a connection status indicator
const connectionStatus = document.querySelector(".connection-status");
const connectionText = document.getElementById("connection-text");

function updateConnectionStatus(status) {
    connectionStatus.classList.remove("connected", "disconnected");

    if (status === "connected") {
        connectionStatus.classList.add("connected");
        connectionText.textContent = "Connected";
    } else if (status === "disconnected") {
        connectionStatus.classList.add("disconnected");
        connectionText.textContent = "Disconnected";
    } else {
        connectionText.textContent = "Connecting...";
    }
}

// The WebSocket to send commands to the server
const socket = new WebSocket(
    `ws://${window.location.host}/ws`
);

socket.addEventListener("open", () => {
    updateConnectionStatus("connected");
    socket.send("STOP");
});

socket.addEventListener("close", () => {
    updateConnectionStatus("disconnected");
    setCommand("STOP");
});

socket.addEventListener("error", () => {
    updateConnectionStatus("disconnected");
    setCommand("STOP");
});




const movementDisplay = document.getElementById("movement");
const buttons = document.querySelectorAll("[data-command]");

let activeCommand = "STOP";
let heldKeys = new Set();

const keyMap = {
    w: "FORWARD",
    ArrowUp: "FORWARD",

    a: "LEFT",
    ArrowLeft: "LEFT",

    s: "BACKWARD",
    ArrowDown: "BACKWARD",

    d: "RIGHT",
    ArrowRight: "RIGHT"
};

function setCommand(command) {
    if (activeCommand === command) return;

    activeCommand = command;

    movementDisplay.textContent =
        command === "STOP" ? "STOPPED" : command;

    buttons.forEach(button => {
        button.classList.toggle(
            "active",
            button.dataset.command === command
        );
    });

    if (socket.readyState === WebSocket.OPEN) {
        socket.send(command);
    }

    console.log("Command:", command);
}

// This runs when a button is pressed
document.addEventListener("keydown", event => {
    const key = event.key.length === 1
    ? event.key.toLowerCase()
    : event.key;

    if (key === " ") {
        event.preventDefault();
        heldKeys.clear();
        setCommand("STOP");
        return;
    }

    if (!(key in keyMap)) return;

    event.preventDefault();
    if (event.repeat) return;

    heldKeys.add(key);
    setCommand(keyMap[key]);
});

// This runs when a button is released
document.addEventListener("keyup", event => {
    const key = event.key.length === 1
    ? event.key.toLowerCase()
    : event.key;

    if (!(key in keyMap)) return;

    heldKeys.delete(key);
    setCommand("STOP");
});

// Mouse and touchscreen controls
buttons.forEach(button => {
    const command = button.dataset.command;

    button.addEventListener("pointerdown", event => {
        event.preventDefault();
        button.setPointerCapture(event.pointerId);
        setCommand(command);
    });

    button.addEventListener("pointerup", () => {
        setCommand("STOP");
    });

    button.addEventListener("pointercancel", () => {
        setCommand("STOP");
    });

    button.addEventListener("lostpointercapture", () => {
        setCommand("STOP");
    });
});

// Stop when leaving the browser tab
window.addEventListener("blur", () => {
    heldKeys.clear();
    setCommand("STOP");
});

document.addEventListener("visibilitychange", () => {
    if (document.hidden) {
        heldKeys.clear();
        setCommand("STOP");
    }
});
