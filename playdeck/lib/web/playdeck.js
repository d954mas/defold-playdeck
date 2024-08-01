var LibPlaydeck = {

    $PlaydeckSdk: {

        _callback: null,

        _call_callback: function (data) {
            data = JSON.stringify(data);
            var _data = stringToNewUTF8(data);
            if (PlaydeckSdk._callback !== null && typeof PlaydeckSdk._callback !== 'undefined') {
                {{{ makeDynCall("vii", "PlaydeckSdk._callback")}}}(_data, lengthBytesUTF8(data));
            } else {
                console.error("PlaydeckSdk._callback is not a exist");
            }
            Module._free(_data);
        }
    },

    HtmlPlaydeckRegisterCallback: function (callback) {
        PlaydeckSdk._callback = callback;
        //post all saved events
        window.playdeck_defold_sdk.game_ready = true;
        window.playdeck_defold_sdk.send = PlaydeckSdk._call_callback;
        for (let i = 0; i < window.playdeck_defold_sdk.events_queue.length; i++) {
            PlaydeckSdk._call_callback(window.playdeck_defold_sdk.events_queue[i]);
        }
        window.playdeck_defold_sdk.events_queue = [];
    },

    HtmlPlaydeckGetUserProfile: function () {
        window.parent.window.postMessage({ playdeck: { method: "getUserProfile" } }, "*");
    },

    HtmlPlaydeckSetData: function (key, value) {
        window.parent.window.postMessage({ playdeck: { method: "setData", key: UTF8ToString(key), value: UTF8ToString(value) } }, "*");
    },

    HtmlPlaydeckGetData: function (key) {
        window.parent.window.postMessage({ playdeck: { method: "getData", key: UTF8ToString(key) } }, "*");
    },

    HtmlPlaydeckCustomShare: function (jsonStr) {
        var value = JSON.parse(UTF8ToString(jsonStr));
        window.parent.window.postMessage({ playdeck: { method: "customShare", value: value } }, "*");
    },

    HtmlPlaydeckGetShareLink: function (jsonStr) {
        var value = JSON.parse(UTF8ToString(jsonStr));
        window.parent.window.postMessage({ playdeck: { method: "getShareLink", value: value } }, "*");
    },

    HtmlPlaydeckOpenTelegramLink: function (url) {
        window.parent.window.postMessage({ playdeck: { method: "openTelegramLink", url: UTF8ToString(url) } }, "*");
    },

    HtmlPlaydeckGetPlaydeckState: function () {
        window.parent.window.postMessage({ playdeck: { method: "getPlaydeckState" } }, "*");
    },

    HtmlPlaydeckGameEnd: function () {
        window.parent.window.postMessage({ playdeck: { method: "gameEnd" } }, "*");
    },

    HtmlPlaydeckSendGameProgress: function (jsonStr) {
        var value = JSON.parse(UTF8ToString(jsonStr));
        window.parent.window.postMessage({ playdeck: { method: "sendGameProgress", value: value } }, "*");
    },

    HtmlPlaydeckSendAnalyticNewSession: function () {
        window.parent.window.postMessage({ playdeck: { method: "sendAnalyticNewSession" } }, "*");
    },

    HtmlPlaydeckSendAnalytics: function (jsonStr) {
        var value = JSON.parse(UTF8ToString(jsonStr));
        window.parent.window.postMessage({ playdeck: { method: "sendAnalytics", value: value } }, "*");
    },

    HtmlPlaydeckSendAnalyticsInternalError: function (errorStr) {
        let eventError = {
            type: 'click',
            user_properties: {},
            event_properties: {
                error: errorStr,
            },
        };
        window.parent.window.postMessage({ playdeck: { method: "sendAnalytics", value: eventError } }, "*");
    },

    HtmlPlaydeckRequestPayment: function (jsonStr) {
        var value = JSON.parse(UTF8ToString(jsonStr));
        window.parent.window.postMessage({ playdeck: { method: "requestPayment", value: value } }, "*");
    },

    HtmlPlaydeckGetPaymentInfo: function (jsonStr) {
        var value = JSON.parse(UTF8ToString(jsonStr));
        window.parent.window.postMessage({ playdeck: { method: "getPaymentInfo", value: value } }, "*");
    },

    HtmlPlaydeckGetToken: function () {
        window.parent.window.postMessage({ playdeck: { method: "getToken" } }, "*");
    },

    HtmlPlaydeckShowAd: function () {
        window.parent.window.postMessage({ playdeck: { method: "showAd" } }, "*");
    },

}

autoAddDeps(LibPlaydeck, '$PlaydeckSdk');
addToLibrary(LibPlaydeck);