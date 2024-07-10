import request from '@/utils/request'

// 查询设备巡检列表
export function listInspection(query) {
  return request({
    url: '/system/inspection/list',
    method: 'get',
    params: query
  })
}

// 查询设备巡检详细
export function getInspection(id) {
  return request({
    url: '/system/inspection/' + id,
    method: 'get'
  })
}

// 新增设备巡检
export function addInspection(data) {
  return request({
    url: '/system/inspection',
    method: 'post',
    data: data
  })
}

// 修改设备巡检
export function updateInspection(data) {
  return request({
    url: '/system/inspection',
    method: 'put',
    data: data
  })
}

// 删除设备巡检
export function delInspection(id) {
  return request({
    url: '/system/inspection/' + id,
    method: 'delete'
  })
}
