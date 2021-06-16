<template>
  <el-calendar>
    <template #dateCell="{ data }">
      <p :class="data.isSelected ? 'is-selected' : ''">
        {{ `${GetLunar(data.date)}` }}
        {{ data.isSelected ? "✔️" : "" }}
      </p>
    </template>
  </el-calendar>
</template>

<script setup>
import { defineComponent, ref } from "vue";
import { Lunar, Solar } from "lunar-javascript";

function GetLunar(date) {
  var lunar = Lunar.fromDate(date),
    solar = Solar.fromDate(date);
  return `${solar.getMonth()}-${solar.getDay()} ${lunar.getMonthInChinese()}月${lunar.getDayInChinese()}`;
}
</script>
<style scoped>
.el-calendar {
  background-color: transparent;
}
</style>