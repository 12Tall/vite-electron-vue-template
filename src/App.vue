<template>
  <Calendar>
    <template v-slot:default="{ data }">
      <!-- 注意插槽在使用时要加花括号 -->
      <div
        class="title"
        :class="{
          weekend: data.week > 5 || data.week < 1,
          today: data.isToday,
          thisMonth: data.month === new Date().getMonth() + 1,
        }"
      >
        <div>{{ GetContent(data) }}</div>
        <div></div>
      </div>
    </template>
  </Calendar>
  <!-- <img alt="Vue logo" src="./assets/logo.png" /> -->
</template>

<script setup>
import { defineComponent, ref } from "vue";
import Calendar from "./components/Calendar.vue";

function GetContent(data) {
  var lunar = data.lunarDay === "初一" ? `${data.lunarMonth}月` : data.lunarDay;
  return `${data.month}-${data.day} ${lunar}`;
}

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
.title {
  border-radius: 5px;
}

/* CSS 的优先级受到排列顺序的影响 */
.weekend {
  color: gold;
}
.thisMonth {
  background-color: #4f4f4f4f;
}
.today {
  color: gold;
  background-color:#8f8f8f8f;
}
</style>
