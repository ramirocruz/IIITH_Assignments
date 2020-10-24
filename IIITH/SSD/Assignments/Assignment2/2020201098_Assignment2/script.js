
$(document).ready(function () {

    if (sessionStorage.navstat) {

        if (sessionStorage.getItem("navstat") == "none") {
            $("#navlist").css("display", "none");

        }
        else {
            $("#navlist").css("display", "flex");

        }
    }
    else {
        console.log("check123");
        $("#navlist").css("display", "none");
        sessionStorage.setItem("navstat", "none");
    }


});

$("#shownav").on("click", function () {
    console.log("Check..");

    if (sessionStorage.getItem("navstat") == "none") {
        $("#navlist").css("display", "flex");
        sessionStorage.setItem("navstat", "flex");
    }
    else {
        $("#navlist").css("display", "none");
        sessionStorage.setItem("navstat", "none");
    }


})

$("#cl1btn").on("click", function () {
    if ($("#cl1").css("display") == "none") {
        $("#cl1").css("display", "block");
        $("#cl1btn").html($(`<em class="fa fa-minus" aria-hidden="true"></em>`));

    }
    else {
        $("#cl1").css("display", "none");
        $("#cl1btn").html($(`<em class="fa fa-plus" aria-hidden="true"></em>`));

    }

})

$("#cl2btn").on("click", function () {
    if ($("#cl2").css("display") == "none") {
        $("#cl2").css("display", "block");
        $("#cl2btn").html($(`<em class="fa fa-minus" aria-hidden="true"></em>`));
    }
    else {
        $("#cl2").css("display", "none");
        $("#cl2btn").html($(`<em class="fa fa-plus" aria-hidden="true"></em>`));
    }

})

$("#cl3btn").on("click", function () {
    if ($("#cl3").css("display") == "none") {
        $("#cl3").css("display", "block");
        $("#cl3btn").html($(`<em class="fa fa-minus" aria-hidden="true"></em>`));
    }
    else {
        $("#cl3").css("display", "none");
        $("#cl3btn").html($(`<em class="fa fa-plus" aria-hidden="true"></em>`));
    }
})

