import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [vue()],
  base: './',  // 默认是打包为绝对路径，如不修改为相对路径，在electron 中会提示找不到资源
})
