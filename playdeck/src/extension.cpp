// myextension.cpp
// Extension lib defines
#define EXTENSION_NAME playdeck
#define LIB_NAME "playdeck"
#define MODULE_NAME "playdeck"
// include the Defold SDK
#include <dmsdk/sdk.h>
#include "utils.h"

#if defined(DM_PLATFORM_HTML5)

typedef void (*PlaydeckCallback)(const char *data, const int length);

extern "C" void HtmlPlaydeckRegisterCallback(PlaydeckCallback callback);
extern "C" void HtmlPlaydeckGetUserProfile();
extern "C" void HtmlPlaydeckSetData(const char *key, const char *value);
extern "C" void HtmlPlaydeckGetData(const char *key);
extern "C" void HtmlPlaydeckCustomShare(const char *jsonStr);
extern "C" void HtmlPlaydeckGetShareLink(const char *jsonStr);
extern "C" void HtmlPlaydeckOpenTelegramLink(const char *url);
extern "C" void HtmlPlaydeckGetPlaydeckState();
extern "C" void HtmlPlaydeckGameEnd();
extern "C" void HtmlPlaydeckSendGameProgress(const char *jsonStr);
extern "C" void HtmlPlaydeckSendAnalyticNewSession();
extern "C" void HtmlPlaydeckSendAnalytics(const char *jsonStr);
extern "C" void HtmlPlaydeckSendAnalyticsInternalError(const char *errorStr);
extern "C" void HtmlPlaydeckRequestPayment(const char *jsonStr);
extern "C" void HtmlPlaydeckGetPaymentInfo(const char *jsonStr);
extern "C" void HtmlPlaydeckGetToken();
extern "C" void HtmlPlaydeckShowAd();
extern "C" void HtmlPlaydeckSetScore(int score, bool force);

inline void convert_lua_table_to_json(lua_State *L, int index, char** json, size_t* json_length) {
    check_table(L, index);
    int ret = dmScript::LuaToJson(L, json, json_length);
    if (ret != 0) {
        luaL_error(L, "Failed to convert lua table to json: %s", lua_tostring(L, -1));
    }
}


struct PlaydeckListener {
    PlaydeckListener() : m_L(0), m_Callback(LUA_NOREF), m_Self(LUA_NOREF) {}
    lua_State *m_L;
    int m_Callback;
    int m_Self;

    // Member function to check if callback and instance are valid
    bool checkCallbackAndInstance() {
        if (m_Callback == LUA_NOREF) {
            dmLogInfo("PlaydeckListener callback does not exist.");
            return false;
        }

        lua_State *L = m_L;
        int top = lua_gettop(L);
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_Callback); // [-1] - callback
        lua_rawgeti(L, LUA_REGISTRYINDEX, m_Self);     // [-1] - self, [-2] - callback
        lua_pushvalue(L, -1);                          // [-1] - self, [-2] - self, [-3] - callback
        dmScript::SetInstance(L);                      // [-1] - self, [-2] - callback

        if (!dmScript::IsInstanceValid(L)) {
            unregisterCallback();
            dmLogError("Could not run PlaydeckListener callback because the instance has been deleted.");
            lua_pop(L, 2);
            assert(top == lua_gettop(L));
            return false;
        }
        return true;
    }

    // Instance method to unregister a callback
    void unregisterCallback() {
        if (m_Callback != LUA_NOREF) {
            dmScript::Unref(m_L, LUA_REGISTRYINDEX, m_Callback);
            dmScript::Unref(m_L, LUA_REGISTRYINDEX, m_Self);
            m_Callback = LUA_NOREF;
            m_Self = LUA_NOREF;
        } else {
            dmLogError("Can't remove a callback that was not registered.");
        }
    }

} PLAYDECK_LISTENER;

static void JsPlaydeck_Callback(const char *data, const int length) {
    lua_State *L = PLAYDECK_LISTENER.m_L;
    int top = lua_gettop(L);
    if (PLAYDECK_LISTENER.checkCallbackAndInstance()) {
        //[-1] - self [-2] - callback
        dmScript::JsonToLua(L, data, length); // throws lua error if it fails
        //[-1] result lua table, [-2] self, [-3] callback
        int ret = lua_pcall(L, 2, 0, 0);
        if (ret != 0) {
            dmLogError("Error running callback: %s", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    assert(top == lua_gettop(L));
}

static int LuaPlaydeckRegisterCallback(lua_State *L) {
    int type = lua_type(L, 1);
    if (type != LUA_TFUNCTION) {
        luaL_error(L, "Playdeck callback is invalid. Use callback function as an argument");
        return 0;
    }

    if (PLAYDECK_LISTENER.m_Callback != LUA_NOREF) {
        return luaL_error(L, "PlaydeckSDK callback already exist.");
    }

    lua_pushvalue(L, 1);
    PLAYDECK_LISTENER.m_Callback = dmScript::Ref(L, LUA_REGISTRYINDEX);

    dmScript::GetInstance(L);
    PLAYDECK_LISTENER.m_Self = dmScript::Ref(L, LUA_REGISTRYINDEX);

    HtmlPlaydeckRegisterCallback((PlaydeckCallback)JsPlaydeck_Callback);
    return 0;
}

static int LuaPlaydeckGetUserProfile(lua_State *L) {
    HtmlPlaydeckGetUserProfile();
    return 0;
}

static int LuaPlaydeckSetData(lua_State *L) {
    HtmlPlaydeckSetData(luaL_checkstring(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int LuaPlaydeckGetData(lua_State *L) {
    HtmlPlaydeckGetData(luaL_checkstring(L, 1));
    return 0;
}

static int LuaPlaydeckCustomShare(lua_State *L) {
    size_t json_length = 0; char* json = 0x0;
    convert_lua_table_to_json(L, 1, &json, &json_length);
    HtmlPlaydeckCustomShare((const char*)json);
    free(json);
    return 0;
}

static int LuaPlaydeckGetShareLink(lua_State *L) {
    size_t json_length = 0; char* json = 0x0;
    convert_lua_table_to_json(L, 1, &json, &json_length);
    HtmlPlaydeckGetShareLink((const char*)json);
    free(json);
    return 0;
}

static int LuaPlaydeckOpenTelegramLink(lua_State *L) {
    HtmlPlaydeckOpenTelegramLink(luaL_checkstring(L, 1));
    return 0;
}

static int LuaPlaydeckGetPlaydeckState(lua_State *L) {
    HtmlPlaydeckGetPlaydeckState();
    return 0;
}

static int LuaPlaydeckGameEnd(lua_State *L) {
    HtmlPlaydeckGameEnd();
    return 0;
}

static int LuaPlaydeckSendGameProgress(lua_State *L) {
    size_t json_length = 0; char* json = 0x0;
    convert_lua_table_to_json(L, 1, &json, &json_length);
    HtmlPlaydeckSendGameProgress((const char*)json);
    free(json);
    return 0;
}

static int LuaPlaydeckSendAnalyticNewSession(lua_State *L) {
    HtmlPlaydeckSendAnalyticNewSession();
    return 0;
}

static int LuaPlaydeckSendAnalytics(lua_State *L) {
    size_t json_length = 0; char* json = 0x0;
    convert_lua_table_to_json(L, 1, &json, &json_length);
    HtmlPlaydeckSendAnalytics((const char*)json);
    free(json);
    return 0;
}

static int LuaPlaydeckRequestPayment(lua_State *L) {
    size_t json_length = 0; char* json = 0x0;
    convert_lua_table_to_json(L, 1, &json, &json_length);
    HtmlPlaydeckRequestPayment((const char*)json);
    free(json);
    return 0;
}

static int LuaPlaydeckGetPaymentInfo(lua_State *L) {
    size_t json_length = 0; char* json = 0x0;
    convert_lua_table_to_json(L, 1, &json, &json_length);
    HtmlPlaydeckGetPaymentInfo((const char*)json);
    free(json);
    return 0;
}

static int LuaPlaydeckGetToken(lua_State *L) {
    HtmlPlaydeckGetToken();
    return 0;
}

static int LuaPlaydeckShowAd(lua_State *L) {
    HtmlPlaydeckShowAd();
    return 0;
}

static int LuaPlaydeckSetScore(lua_State *L) {
    HtmlPlaydeckSetScore(lua_tonumber(L, 1), lua_toboolean(L, 2));
    return 0;
}

static const luaL_reg Module_methods[] = {
    {"register_callback", LuaPlaydeckRegisterCallback},
    {"get_user_profile", LuaPlaydeckGetUserProfile},
    {"set_data", LuaPlaydeckSetData},
    {"get_data", LuaPlaydeckGetData},
    {"custom_share", LuaPlaydeckCustomShare},
    {"get_share_link", LuaPlaydeckGetShareLink},
    {"open_telegram_link", LuaPlaydeckOpenTelegramLink},
    {"get_playdeck_state", LuaPlaydeckGetPlaydeckState},
    {"game_end", LuaPlaydeckGameEnd},
    {"send_game_progress", LuaPlaydeckSendGameProgress},
    {"send_analytic_new_session", LuaPlaydeckSendAnalyticNewSession},
    {"send_analytics", LuaPlaydeckSendAnalytics},
    {"request_payment", LuaPlaydeckRequestPayment},
    {"get_payment_info", LuaPlaydeckGetPaymentInfo},
    {"get_token", LuaPlaydeckGetToken},
    {"show_ad", LuaPlaydeckShowAd},
    {0, 0}};

static void LuaInit(lua_State *L) {
    int top = lua_gettop(L);
    luaL_register(L, MODULE_NAME, Module_methods);
    lua_pop(L, 1);
    PLAYDECK_LISTENER.m_L = L;
    assert(top == lua_gettop(L));
}

static void LogListener(LogSeverity severity, const char* domain, const char* formatted_string){
    switch (severity){
        case LOG_SEVERITY_ERROR:
        case LOG_SEVERITY_FATAL:
            HtmlPlaydeckSendAnalyticsInternalError(formatted_string);
        break;
        case LOG_SEVERITY_WARNING:
        case LOG_SEVERITY_DEBUG:
        case LOG_SEVERITY_USER_DEBUG:
        case LOG_SEVERITY_INFO:
        // do nothing
        break;
        default:
        // do nothing
        break;
    }
}

static dmExtension::Result AppInitializeExtension(dmExtension::AppParams *params){
    dmLogRegisterListener(&LogListener);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result AppFinalizeExtension(dmExtension::AppParams *params){
    dmLogUnregisterListener(&LogListener);
    return dmExtension::RESULT_OK;
}

static dmExtension::Result InitializeExtension(dmExtension::Params *params) {
    LuaInit(params->m_L);
    printf("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}


DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeExtension, AppFinalizeExtension, InitializeExtension, 0, 0, 0)

#else

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, 0, 0, 0, 0, 0, 0)

#endif