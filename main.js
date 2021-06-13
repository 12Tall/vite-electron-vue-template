const { app, BrowserWindow, screen, Menu, Tray } = require('electron'),
  user32 = require("user32-napi"),
  path = require("path")
const isDev = process.env.DEV === "DEV";

let tray = null;  // 声明全局变量，否则托盘程序会自动消失
// https://www.codenong.com/cs109046186/

// let url = isDev ? 'http://localhost:5000/' : `file://${process.cwd()}/resources/app/dist/index.html`;
let url = isDev ? 'http://localhost:5000/' : `file://${process.cwd()}/resources/app.asar/dist/index.html`;
// 注意这里的路径对打包后程序能否正常运行影响重大

app.on('ready', () => {
  let { width, height } = screen.getPrimaryDisplay().workAreaSize;

  window = new BrowserWindow({
    width: width,
    height: height,
    x: 0,
    y: 0,
    resizable: false,
    movable: false,
    minimizable: false,
    type: "toolbar",
    frame: false,
    transparent: true,  // 窗口透明，解决程序退出边框遗留问题


    // alwaysOnTop: true
  })
  window.loadURL(url)


  parent = user32.getWallpaperWindow();
  wallpaper = window.getNativeWindowHandle().readInt32LE();
  user32.setParent(wallpaper, parent)

  // 系统托盘
  var icon = isDev ? './app.png' : path.join(path.dirname(app.getPath('exe')), '/resources/app.asar/app.png');
  tray = new Tray(icon);
  const contextMenu = Menu.buildFromTemplate([{
    label: "编辑",
    type: "checkbox",
    checked: false,
    click: function (item) {
      parent = item.checked ? null : user32.getWallpaperWindow();
      wallpaper = window.getNativeWindowHandle().readInt32LE();
      user32.setParent(wallpaper, parent);
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
  // window.webContents.openDevTools({
  //   mode: 'right'
  // });
})

// 开机自动登录
// app.setLoginItemSettings({
//   openAtLogin: true
// })

// 解决程序退出不彻底的问题
app.on('window-all-closed', event => {
  app.exit();
});
