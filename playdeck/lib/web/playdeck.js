var LibPlaydeck = {

    $PlaydeckSdk: {

        _callback: null,
        _messagesHashTable: {},

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

    HtmlPlaydeckLoading: function (value) {
        window.parent.window.postMessage({ playdeck: { method: "loading", value: value } }, "*");
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

    HtmlPlaydeckSetDataString: function (key, value) {
        window.parent.window.postMessage({ playdeck: { method: "setData", key: UTF8ToString(key), value: UTF8ToString(value) } }, "*");
    },
    HtmlPlaydeckSetDataBoolean: function (key, value) {
        window.parent.window.postMessage({ playdeck: { method: "setData", key: UTF8ToString(key), value: value } }, "*");
    },
    HtmlPlaydeckSetDataNumber: function (key, value) {
        window.parent.window.postMessage({ playdeck: { method: "setData", key: UTF8ToString(key), value: value === 1 } }, "*");
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

    HtmlPlaydeckSendAnalyticsInternalError: function (formatted_string) {
        var js_formatted_string = UTF8ToString(formatted_string);
        if (PlaydeckSdk._messagesHashTable[js_formatted_string]) {
            return;
        }
        PlaydeckSdk._messagesHashTable[js_formatted_string] = true;
        var final_message = "DEFOLD:" + js_formatted_string;

        let eventError = {
            type: 'click',
            user_properties: {},
            event_properties: {
                name : "defold_error",
                defold_error: final_message,
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