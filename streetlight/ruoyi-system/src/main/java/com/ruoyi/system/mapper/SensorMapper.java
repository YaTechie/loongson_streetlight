package com.ruoyi.system.mapper;


import org.apache.ibatis.annotations.*;

@Mapper
public interface SensorMapper {

    @Select("SELECT COUNT(*) FROM z_device WHERE device_id = #{deviceId}")
    int deviceExists(String deviceId);

    @Insert("INSERT INTO z_device (windspeed) VALUES (#{data}) WHERE device_id = #{deviceId}")
    int insertWindSpeed(@Param("deviceId") String deviceId, @Param("data") double data);

    @Update("UPDATE z_device SET windspeed = #{data} WHERE device_id = #{deviceId}")
    int updateWindSpeed(@Param("deviceId") String deviceId, @Param("data") double data);

    @Insert("INSERT INTO z_device (sensor_pm2) VALUES (#{data}) WHERE device_id = #{deviceId}")
    int insertPm2(@Param("deviceId") String deviceId, @Param("data") double data);

    @Update("UPDATE z_device SET sensor_pm2 = #{data} WHERE device_id = #{deviceId}")
    int updatePm2(@Param("deviceId") String deviceId, @Param("data") double data);

    @Insert("INSERT INTO z_device (direction, angle) VALUES (#{direction}, #{angle}) WHERE device_id = #{deviceId}")
    int insertDirection(@Param("deviceId") String deviceId, @Param("direction") String direction, @Param("angle") int angle);

    @Update("UPDATE z_device SET direction = #{direction}, angle = #{angle} WHERE device_id = #{deviceId}")
    int updateDirection(@Param("deviceId") String deviceId, @Param("direction") String direction, @Param("angle") int angle);

    @Insert("INSERT INTO z_device (sensor_humidity, sensor_temperature, sensor_light) VALUES (#{humidity}, #{temperature}, #{light}) WHERE device_id = #{deviceId}")
    int insertThl(@Param("deviceId") String deviceId, @Param("humidity") double humidity, @Param("temperature") double temperature, @Param("light") double light);

    @Update("UPDATE z_device SET sensor_humidity = #{humidity}, sensor_temperature = #{temperature}, sensor_light = #{light} WHERE device_id = #{deviceId}")
    void updateThl(@Param("deviceId") String deviceId, @Param("humidity") double humidity, @Param("temperature") double temperature, @Param("light") double light);

    @Insert("INSERT INTO z_device (sensor_pwm) VALUES (#{data}) WHERE device_id = #{deviceId}")
    int insertPWM(@Param("deviceId") String deviceId, @Param("data") int data);

    @Update("UPDATE z_device SET sensor_pwm = #{data} WHERE device_id = #{deviceId}")
    int updatePWM(@Param("deviceId") String deviceId, @Param("data") int data);
}

