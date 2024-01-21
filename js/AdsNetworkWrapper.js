var adsWrapper =
    {
        $adsLocal: {
            onMraidAdsSdkReady : function (w4Self, callback)
            {
                /** MRAID **/

                if (typeof mraid !== "undefined")
                {
                    mraid.addEventListener
                    (
                        'viewableChange',
                        function (viewable)
                        {
                            if (viewable) { wasmTable.get(callback)(w4Self, true); }
                            else {  wasmTable.get(callback)(w4Self, false); }
                        }
                    );
                }
            },

            onIronsourceAdsSdkReady : function (w4Self, callbackviewableChange, callbackAudioVolumeChanged)
            {
                /** Ironsource **/
                if (typeof  dapi  != "undefined")
                {
                    //dapi.removeEventListener("ready", adsLocal.onIronsourceAdsSdkReady);

                    dapi.getScreenSize();
                    dapi.getAudioVolume();

                    if (typeof callbackAudioVolumeChanged !== "undefined")
                    {
                        dapi.addEventListener(
                            "audioVolumeChange",
                            function (event) {
                                wasmTable.get(callbackAudioVolumeChanged)(w4Self, event);
                            }
                        );
                    }

                    if (typeof callbackviewableChange !== "undefined")
                    {
                        dapi.addEventListener(
                            "viewableChange",
                            function (event) {
                                if (event.isViewable) {
                                    wasmTable.get(callbackviewableChange)(w4Self, true);
                                } else {
                                    wasmTable.get(callbackviewableChange)(w4Self, false);
                                }
                            }
                        );
                    }

                }
            }
        },

        w4AdsStart : function ()
        {
            /** MRAID **/
            if (typeof mraid !== "undefined")
            {
                if (typeof TARGET_URL === "undefined")
                {
                    console.error("TARGET_URL is undef, set https://www.space.com/");
                    TARGET_URL = "https://www.space.com/";
                }
            }
        },

        w4AdsComplete : function ()
        {
            /** Vungle **/
            if (typeof parent !== "undefined" && parent != null)
            {
                parent.postMessage('complete','*');
            }

        },

        w4AdsClose : function ()
        {
            /** MRAID **/
            if (typeof mraid !== "undefined")
            {
                mraid.close();
            }
        },

        w4AdsUseCustomCloseButton : function ( flag )
        {
            /** MRAID **/
            if (typeof mraid !== "undefined")
            {
                mraid.useCustomClose(flag);
            }
        },

        w4AdsIsCustomButtonAvalible : function (  )
        {
            /** MRAID **/
            if (typeof mraid !== "undefined")
            {
                return true;
            }

            return false;
        },

        w4AdsCallCTA:function ( )
        {
            /** MRAID **/
            if (typeof mraid !== "undefined")
            {
                mraid.open(TARGET_URL);
            }

            /** Google Ads**/
            if (typeof ExitApi !==  "undefined")
            {
                ExitApi.exit();
            }

            /** Facebook **/
            if (typeof FbPlayableAd !== "undefined")
            {
                FbPlayableAd.onCTAClick();
            }

            /** Vungle **/
            if (typeof parent !== "undefined" && parent != null)
            {
                parent.postMessage('download','*');
            }

            /** Ironsource **/
            if (typeof dapi !== "undefined")
            {
                dapi.openStoreUrl();
            }

            /** LiftOff **/
            if (typeof Liftoff !== "undefined")
            {
                Liftoff.open();
            }
        },

        w4AdsAudioVolumeChangeHandler: function (w4Self, callback)
        {
            /** Ironsource **/
            if (typeof dapi !== "undefined")
            {
                if (!dapi.isReady())
                {
                    console.log("!dapi.isReady()");
                    dapi.addEventListener("ready", function () { adsLocal.onIronsourceAdsSdkReady(w4Self, null, callback);});
                }
                else
                {
                    console.log("dapi.isReady()");
                    adsLocal.onIronsourceAdsSdkReady(w4Self, null, callback);
                }
            }
        },
        w4AdsVisibleChangeHandler: function (w4Self, callback)
        {
            /** MRAID **/
            if (typeof mraid !== "undefined")
            {
                if (mraid.getState() === 'loading')
                {
                    mraid.addEventListener('ready', function () { adsLocal.onMraidAdsSdkReady (w4Self, callback);});
                }
                else
                {
                    adsLocal.onMraidAdsSdkReady(w4Self, callback);
                }
            }

            /** Vungle **/
            if (typeof parent !== "undefined" && parent != null)
            {
                window.addEventListener('ad-event-pause', function () { wasmTable.get(callback)(w4Self, false); });
                window.addEventListener('ad-event-resume', function () { wasmTable.get(callback)(w4Self, true); });
            }

            /** Ironsource **/
            if (typeof dapi !== "undefined")
            {
                if (!dapi.isReady())
                {
                    console.log("!dapi.isReady()");
                    dapi.addEventListener("ready", function () { adsLocal.onIronsourceAdsSdkReady(w4Self, callback, null);});
                }
                else
                {
                    console.log("dapi.isReady()");
                    adsLocal.onIronsourceAdsSdkReady(w4Self, callback, null);
                }
            }

        }
    };

autoAddDeps(adsWrapper, '$adsLocal');
mergeInto(LibraryManager.library, adsWrapper);
