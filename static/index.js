function maximizeImage(id) {
    let img = document.getElementById(id);
    let isIOS = /iPad|iPhone|iPod/.test(navigator.userAgent) || 
                (navigator.userAgentData && navigator.userAgentData.platform === "iOS");
    if (isIOS) {
        alert("Fullscreen mode is not supported on iOS. Please rotate your device for a better view.");
        return;
    }
    if (img.requestFullscreen) {
        img.requestFullscreen().then(() => {
            if (screen.orientation && screen.orientation.lock) {
                screen.orientation.lock("landscape").catch((err) => {
                    console.warn("Screen rotation not allowed:", err);
                });
            }
        }).catch(err => console.warn("Fullscreen request failed:", err));
    } else if (img.mozRequestFullScreen) { 
        img.mozRequestFullScreen();
    } else if (img.webkitRequestFullscreen) { 
        img.webkitRequestFullscreen();
    } else if (img.msRequestFullscreen) { 
        img.msRequestFullscreen();
    } else {
        alert("Fullscreen mode is not supported on your device.");
    }
}

  /* juls */