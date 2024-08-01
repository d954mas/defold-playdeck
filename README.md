[![GitHub release (latest by date)](https://img.shields.io/github/v/release/d954mas/defold-playdeck)](https://github.com/d954mas/defold-playdeck/releases)
[![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/d954mas/defold-playdeck/bob.yml?branch=master)](https://github.com/d954mas/defold-playdeck)
# PlayDeck for Defold

PlayDeck is a Telegram-native gaming marketplace that empowers game developers to showcase their games on one of the fastest-growing social platforms. With PlayDeck toolkit solutions, you can launch Web2 and Web3 games on Telegram and use all the benefits of social and referral mechanics to boost traffic and revenue.

[More info](https://playdeck.io)

[Official Integration Guide(JS)](https://github.com/ton-play/playdeck-integration-guide)

---
## Setup

### Dependency

You can use the **playdeck** extension in your own project by adding this project as a [Defold library dependency](https://www.defold.com/manuals/libraries/). Open your game.project file and in the dependencies field under project add:

> https://github.com/d954mas/defold-playdeck/archive/refs/tags/1.0.0.zip


---
## How to use
1. Look at official integration guide [Official Integration Guide(JS)](https://github.com/ton-play/playdeck-integration-guide)

2. Loding message will be send automatically.
```
// We auto-report progress based on the engine loading, if your game needs additional time to load assets,
// you can comment this out and report progress manually so the game does not start before it's ready.
if (Progress) {
    Progress.addListener(val => {
        window.parent.window.postMessage({ playdeck: { method: "loading", value: val } }, "*");
    });
}
```

3. When your game is loaded register callback
```
playdeck.register_callback(function(self, data)
    pprint(data) -- log received data

    if data.method == "loaded" then
		
    elseif data.method == "getData" then
		
    elseif data.method == "play" then
	
    elseif (data.method == "rewardedAd") then
		
    elseif (data.method == "errAd") then
		
    elseif (data.method == "skipAd") then
		
    elseif (data.method == "notFoundAd") then
	
    elseif (data.method == "startAd") then
		
    ...
end)
```


---
## API

For details look in [Official Integration Guide(JS)](https://github.com/ton-play/playdeck-integration-guide)

1. Register Callback
```lua
playdeck.register_callback(function(self, data)
    pprint(data) -- log received data
    ...
```

2. GetUserProfile
```lua
playdeck.get_user_profile()
```

```lua
--callback
if data.method == "getUserProfile" then
    print(data.value)
end
```

3. GetData/SetData
```lua
playdeck.get_data("storage")
playdeck.set_data("storage", json.encode(lua_table))
```

```lua
--callback
if data.method == "getData" then
    if data.value.data then
        local lua_table = JSON.decode(data.value.data)
    else
        print("no data")
    end
end
```

4. CustomShare.
```lua
playdeck.custom_share(lua_table)
```

5. GetShareLink.
```lua
playdeck.get_share_link(lua_table)
```

```lua
--callback
if data.method == "getShareLink" then
    print(data.value)
end
```

6. OpenTelegramLink.
```lua
playdeck.open_telegram_link(url)
```

7. GetPlaydeckState.
```lua
playdeck.get_playdeck_state()
```

```lua
--callback
if data.method == "getPlaydeckState" then
    local isPlayDeckOpened = playdeck.value
end
```

8. GameEnd.
```lua
playdeck.game_end()
```

9. SendGameProgress.
```lua
playdeck.send_game_progress(lua_table)
```

10. SendAnalyticNewSession.
```lua
playdeck.send_analytic_new_session()
```

11. SendAnalytics.
```lua
playdeck.send_analytics(lua_table)
```

12. RequestPayment.
```lua
playdeck.request_payment(lua_table)
```

```lua
--callback
if data.method == "requestPayment" then
    pprint(data.value) -- { url: 'https://t.me/$XIVLvBpfOEsBBwAARs....' } // payment link
    --How to open links is described in the chapter Sharing and opening links.
    playdeck.open_telegram_link(data.value.url)
elseif (data.method == 'invoiceClosed') {
    pprint(playdeck.value) -- { status: 'paid' | 'cancelled' | 'failed' | 'pending' }
end
```

13. GetPaymentInfo.
```lua
playdeck.get_payment_info(lua_table)
```

```lua
--callback
if data.method == "getPaymentInfo" then
    pprint(data.value) -- { status: 'paid' | 'cancelled' | 'failed' | 'pending' }
end
```

14. GetToken.
```lua
playdeck.get_token()
```

```lua
--callback
if data.method == "getToken" then
    pprint(data.value) -- token { token: '123456789...' }
end
```

15. ShowAd.
```lua
playdeck.show_ad()
```

```lua
--callback
if data.method == "startAd" then
    pprint(data.value) --user has been started watching ad
elseif data.method == "rewardedAd" then
    pprint(data.value) --user has been watched ad
elseif data.method == "errAd" then
    pprint(data.value) --something went wrong at advert provider
elseif data.method == "skipAd" then
    pprint(data.value) --user has been skip ad
elseif data.method == "notFoundAd" then
    pprint(data.value) --advert provider doesn't return any ad
end


