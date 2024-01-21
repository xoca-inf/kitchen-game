var empty =
    {
        getWindowSizeWidth :function()
        {
            var clientWidth = document.body.clientWidth;
            if(clientWidth !=0)
            {
                return clientWidth;
            }
            return window.screen.width;
        },
        getWindowSizeHeight :function()
        {
            var clientHeight = document.body.clientHeight;
            if (clientHeight != 0) {
                return clientHeight;
            }
            return window.screen.height;
        }
    };

mergeInto(LibraryManager.library, empty);
