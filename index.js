const change = () => {
    const div = document.getElementById("pussy");

    const to_pussy = div => {
        div.innerHTML = "pussy";
        div.className = "pussy-class";
    };

    const to_fag = div => {
        div.innerHTML = "fag";
        div.className = "fag-class";
    };

    if (div.innerHTML == "pussy")
        to_fag(div);
    else
        to_pussy(div);
};

setInterval(change, 500);
