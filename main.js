const { app, BrowserWindow, screen, Menu, Tray, Notification, ipcMain } = require('electron'),
  user32 = require("user32-napi"),
  path = require("path"),
  schedule = require('node-schedule');
const isDev = process.env.DEV === "DEV";

let tray = null,  // 声明全局变量，否则托盘程序会自动消失
  // https://www.codenong.com/cs109046186/
  onClock = null



// let url = isDev ? 'http://localhost:5000/' : `file://${process.cwd()}/resources/app/dist/index.html`;
let url = isDev ? 'http://localhost:5000/' : `file://${process.cwd()}/resources/app.asar/dist/index.html`;
// 注意这里的路径对打包后程序能否正常运行影响重大

app.name = "动态壁纸"
app.on('ready', () => {
  onClock = new Notification({ title: "整点报时", timeoutType: 'default' })




  let { width, height } = screen.getPrimaryDisplay().workAreaSize,
    w = 1165,
    h = 720;
  window = new BrowserWindow({
    width: w - 5,
    height: h,
    x: width - w,
    y: height - h,
    resizable: false,
    movable: false,
    minimizable: false,
    // type: "toolbar",
    frame: false,
    transparent: true,  // 窗口透明，解决程序退出边框遗留问题
    // https://stackoverflow.com/questions/44391448/electron-require-is-not-defined
    webPreferences: {
      nodeIntegration: false,  // 为true 的话存在安全问题
      contextIsolation: true,  // 防止原型污染  
      enableRemoteModule: false,  // 禁用远程模块
      preload: isDev ? `${__dirname}/preload.js` : path.join(path.dirname(app.getPath('exe')), '/resources/app.asar/preload.js')
    }

    // alwaysOnTop: true
  })
  window.loadURL(url)
  const job = schedule.scheduleJob('0/30 * * * *', function () {
    var _time = new Date();
    onClock.body = `${_time}`
    window.webContents.send("readText", `现在是北京时间 ${_time.getHours()} 点 ${_time.getMinutes()} 分整`)
    onClock.show();
  });

  let parent = user32.getWallpaperWindow(),
    wallpaper = window.getNativeWindowHandle().readInt32LE();
  user32.setParent(wallpaper, parent);


  // 系统托盘
  var icon = isDev ? './app.png' : path.join(path.dirname(app.getPath('exe')), '/resources/app.asar/app.png');
  tray = new Tray(icon);
  const contextMenu = Menu.buildFromTemplate([{
    label: "编辑",
    type: "checkbox",
    checked: false,
    click: function (item) {
      var father = item.checked ? null : parent;
      // wallpaper = window.getNativeWindowHandle().readInt32LE();
      user32.setParent(wallpaper, father);
      // user32.setParent(father, null);
      user32.setWindowTransparent(wallpaper);
      // user32.updateWindow(wallpaper);
      // parent = user32.getWallpaperWindow();
    }
  }, {
    label: '退出',
    click: function () {
      window.close();
      app.quit()
    }
  },
  ]);
  tray.setToolTip('cal');
  tray.setContextMenu(contextMenu);

  // 开发者工具
  // window.webContents.openDevTools({
  //   mode: 'right'
  // });


  // 添加事件透传，但是好像是用不上了
  // https://www.electronjs.org/docs/api/frameless-window#forwarding
  // ipcMain.on('forward-mouse-events', (event, ...args) => {
  //   window.setIgnoreMouseEvents(...args)
  // })

})

// 开机自动登录
// app.setLoginItemSettings({
//   openAtLogin: true
// })

// 解决程序退出不彻底的问题
app.on('window-all-closed', event => {
  app.exit();
});

