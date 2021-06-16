const { contextBridge, ipcRenderer } = require('electron')


// 直接定义window.ipcRenderer 会显示undefined
// https://www.electronjs.org/docs/tutorial/process-model#preload-scripts
contextBridge.exposeInMainWorld(
    "api", {
    send: (channel, data) => {
        // 消息白名单
        let validChannels = ["readText"];
        if (validChannels.includes(channel)) {
            ipcRenderer.send(channel, data);
        }
    },
    receive: (channel, func) => {
        let validChannels = ["readText"];        
        // 消息过滤
        if (validChannels.includes(channel)) {
            ipcRenderer.on(channel, (event, ...args) => func(...args));
        }
    }
}
);

