# vite-electron-vue-template
Electron app template with vite and vue.

## 搭建步骤
除直接克隆本项目外，还可以选择自己动手搭建
### 1. 项目初始化
```bash
yarn create @vitejs/vue
    # 这里选择了vue+js

yarn add electron 

yarn add concurrently
yarn add wait-on  
# 上面两个包是为了在调试时保持代码热更新

yarn add electron-packager
# 打包工具
```

### 2. 修改配置文件
`package.json` 中要修改的不多：
```js
{ 
  // ...
  "main": "main.js",  // 入口文件
  "scripts": {
    "dev": "concurrently -k \"vite --port 5000\" \"wait-on http://localhost:5000/index.html&& set NODE_ENV=DEV&& electron .\"",
    // 同时启动两个命令：1，启动vite 监听5000 端口；2，等待vite 启动后，执行electron .
    "dist": "vite build&&electron-packager . vite-electron-vue-template --platform=win32 --arch=x64 --icon=app.ico --out=./out --app-version=0.0.1 --asar --overwrite --ignore=node_modules"
    // 打包：注意--asar 可以将源代码加密
  },
  // ...
}
```
`vite.config.js` 中需要修改`vue` 打包规则：使用相对路径：
```js
export default defineConfig({
  plugins: [vue()],
  base: './',  // 默认是打包为绝对路径，如不修改为相对路径，在electron 中会提示找不到资源
})
```
`main.js` 入口文件处，应将调试模式单独考虑：
```js
const electron = require('electron')
const app = electron.app
const BrowserWindow = electron.BrowserWindow

// 此代码修改自https://medium.com/@mikeal/vue-js-electron-the-easy-way-adc3ca09234a
let url
if (process.env.NODE_ENV === 'DEV') {
  url = 'http://localhost:5000/'
} else {
  url = `file://${process.cwd()}/resources/app.asar/dist/index.html`
}

app.on('ready', () => {
  let window = new BrowserWindow({width: 800, height: 600})
  window.loadURL(url)
})
```

程序打包时的配置比较简陋，只保证能跑，更高级的功能需要自己去探索了。

## 参考资料
1. [Vue.js & Electron: The easy way.](https://medium.com/@mikeal/vue-js-electron-the-easy-way-adc3ca09234a)
1. [electron-builder打包指定资源到安装目录](https://www.cnblogs.com/mrwh/p/12961446.html)  
1. [Electron打包后获取不到系统托盘图标](https://segmentfault.com/q/1010000022293025)
