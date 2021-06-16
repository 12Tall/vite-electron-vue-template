<template>
  <Calendar></Calendar>
  <!-- <img alt="Vue logo" src="./assets/logo.png" /> -->
</template>

<script setup>
import { defineComponent, ref } from "vue";
import Calendar from "./components/Calendar.vue";

// export default defineComponent({
//   setup() {
//     return {
//       Calendar,
//     };
//   },
// });

// This starter template is using Vue 3 experimental <script setup> SFCs
// Check out https://github.com/vuejs/rfcs/blob/script-setup-2/active-rfcs/0000-script-setup.md
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  background-color: #00000000;

  position: absolute;
  top: 0;
  left: 0;
  bottom: 0;
  right: 0;
}
</style>
