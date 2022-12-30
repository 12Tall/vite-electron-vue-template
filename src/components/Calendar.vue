<template>
  <div class="calendar">
    <div class="calendar_title"></div>
    <div class="calendar_row" v-for="r in row" :key="'row_' + r">
      <div class="calendar_col" v-for="c in col" :key="'col_' + c">
        <div class="calendar_cell">
          <!-- {{today}} -->
          <slot :data="GetDate(r * 7 + c - 8)"></slot>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, reactive } from "vue";
import { Solar } from "lunar-javascript";



let row = ref(6),
  col = ref(7),
  today = ref(new Date()),
  firstDayOfMonth = new Date(new Date().setDate(0)),
  startDay = reactive(
    Solar.fromDate(new Date(new Date().setDate(-firstDayOfMonth.getDay())))
  );

window.api.receive("readText", (message) => {
  today.value = new Date();
  console.log(today)
});

function GetDate(index) {
  var solar = startDay.next(index),
    lunar = solar.getLunar(),
    month = solar.getMonth(),
    day = solar.getDay(),
    week = solar.getWeek();
  // console.log(day, today.getDate(), month, today.getMonth());
  return {
    month,
    day,
    week,
    isToday: day === today.value.getDate() && month === today.value.getMonth() + 1,
    lunarMonth: lunar.getMonthInChinese(),
    lunarDay: lunar.getDayInChinese(),
  };
}
</script>
<style scoped>
.calendar {
  background-color: transparent;
  height: 705px;
  width: 1100px;
  margin: 10px auto 10px auto;
  display: flex;
  flex-direction: column;
  flex-basis: 5px;
  justify-content: space-between;
  color: aliceblue;
}

.calendar_title {
  height: 100px;
  background-color: transparent;
}

.calendar_row {
  height: 98px;
  display: flex;
  justify-content: space-between;
}

.calendar_col {
  display: inline-block;
  width: 155px;
  padding: 5px, 5px, 5px, 5px;
}

.calendar_cell {
  display: inline-block;
  text-align: center;
  width: 100%;
  height: 100%;
  box-sizing: border-box;
  border-radius: 5px;
  box-shadow: 0 0 0 1px hsla(240, 0%, 100%, 0.2) inset,
    0 0.5em 1em rgba(0, 0, 0, 0.4);
}
</style>