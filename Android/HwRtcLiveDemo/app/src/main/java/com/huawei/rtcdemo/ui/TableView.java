/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package com.huawei.rtcdemo.ui;

import android.util.AttributeSet;
import android.view.View;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;

import android.content.Context;
import android.content.res.TypedArray;

import com.huawei.rtcdemo.R;

import java.util.ArrayList;
import java.util.List;

/**
 * 调试信息显示视图
 */
public class TableView extends View {
    /**
     * 单元格尺寸
     */
    private float cellColW;
    private float cellRowH;

    /**
     * 分割线宽度和颜色
     */
    private float divW;
    private int divColor;

    /**
     * 字体大小和颜色
     */
    private float normalTextSize;
    private int normalTextColor;

    /**
     * 表头背景颜色和字体大小颜色
     */
    private int tabHeaderColor;
    private float tabHeaderTextSize;
    private int tabHeaderTextColor;

    /**
     * 行数和列数
     */
    private int cellRowCount;
    private int cellColCount;

    /**
     * 画笔工具
     */
    private Paint myPaint;

    private float[] cellColLefts;
    private float[] cellColWidths;

    private int[] cellColWeights;
    private List<String[]> tableContentList;

    public TableView(Context context) {
        super(context);
        init(null);
    }

    public TableView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(attrs);
    }

    /**
     * 初始化
     *
     * @param attrs attrs
     */
    private void init(AttributeSet attrs) {
        myPaint = new Paint();
        myPaint.setAntiAlias(true);
        myPaint.setTextAlign(Paint.Align.CENTER);
        tableContentList = new ArrayList<>();
        if (attrs != null) {
            TypedArray typedArray = getContext().obtainStyledAttributes(attrs, R.styleable.TableView);
            cellColW = typedArray.getDimensionPixelSize(R.styleable.TableView_unitColumnWidth, 0);
            cellRowH = typedArray.getDimensionPixelSize(R.styleable.TableView_rowHeight, dip2px(36));
            divW = typedArray.getDimensionPixelSize(R.styleable.TableView_dividerWidth, 1);
            divColor = typedArray.getColor(R.styleable.TableView_dividerColor, Color.parseColor("#E1E1E1"));
            normalTextSize = typedArray.getDimensionPixelSize(R.styleable.TableView_textSize, dip2px(10));
            normalTextColor = typedArray.getColor(R.styleable.TableView_textColor, Color.parseColor("#999999"));
            tabHeaderColor = typedArray.getColor(R.styleable.TableView_headerColor, Color.parseColor("#00ffffff"));
            tabHeaderTextSize = typedArray.getDimensionPixelSize(R.styleable.TableView_headerTextSize, dip2px(10));
            tabHeaderTextColor = typedArray.getColor(R.styleable.TableView_headerTextColor, Color.parseColor("#999999"));
            typedArray.recycle();
        } else {
            cellColW = 0;
            cellRowH = dip2px(36);
            divW = 1;
            divColor = Color.parseColor("#E1E1E1");
            normalTextSize = dip2px(10);
            normalTextColor = Color.parseColor("#999999");
            tabHeaderColor = Color.parseColor("#00ffffff");
            tabHeaderTextSize = dip2px(10);
            tabHeaderTextColor = Color.parseColor("#111111");
        }
        setTableHeader("Header1", "Header2").addTableContent("Column1", "Column2");
        initTableDataSize();
    }

    /**
     * dp转px
     *
     * @param dipValue dp单位
     * @return px单位
     */
    public int dip2px(float dipValue) {
        return (int) (dipValue * getContext().getResources().getDisplayMetrics().density);
    }

    /**
     * 测量
     *
     * @param widthMeasureSpec  宽度
     * @param heightMeasureSpec 高度
     */
    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        int weightSum = 0;
        if (cellColWeights != null) {
            for (int i = 0; i < cellColCount; i++) {
                if (cellColWeights.length > i) {
                    weightSum += cellColWeights[i];
                } else {
                    weightSum += 1;
                }
            }
        } else {
            weightSum = cellColCount;
        }

        float width;
        if (cellColW == 0) {
            super.onMeasure(widthMeasureSpec, heightMeasureSpec);
            width = getMeasuredWidth();
            cellColW = (width - (cellColCount + 1) * divW) / weightSum;
        } else {
            width = divW * (cellColCount + 1) + cellColW * weightSum;
        }
        float height = (divW + cellRowH) * cellRowCount + divW;
        setMeasuredDimension((int) width, (int) height);
    }

    /**
     * 绘制
     *
     * @param canvas 画布
     */
    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        calculateTableCol();
        drawTableHeader(canvas);
        drawTableFramework(canvas);
        drawTableContent(canvas);
    }

    /**
     * 绘制表头
     *
     * @param canvas 画布
     */
    private void drawTableHeader(Canvas canvas) {
        myPaint.setColor(tabHeaderColor);
        canvas.drawRect(divW, divW, getWidth() - divW, cellRowH + divW, myPaint);
    }

    /**
     * 绘制表格基础框架
     *
     * @param canvas 画布
     */
    private void drawTableFramework(Canvas canvas) {
        myPaint.setColor(divColor);
        for (int i = 0; i < cellColCount + 1; i++) {
            if (i == 0) {
                canvas.drawRect(0, 0, divW, getHeight(), myPaint);
                continue;
            }
            if (i == cellColCount) {
                canvas.drawRect(getWidth() - divW, 0, getWidth(), getHeight(), myPaint);
                continue;
            }
            canvas.drawRect(cellColLefts[i], 0, cellColLefts[i] + divW, getHeight(), myPaint);
        }
        for (int i = 0; i < cellRowCount + 1; i++) {
            canvas.drawRect(0, i * (cellRowH + divW), getWidth(), i * (cellRowH + divW) + divW, myPaint);
        }
        this.getRootView().requestLayout();
    }

    /**
     * 绘制表格数据具体内容
     *
     * @param canvas 画布
     */
    private void drawTableContent(Canvas canvas) {
        for (int i = 0; i < cellRowCount; i++) {
            String[] rowContent = new String[0];
            if (tableContentList.size() > i && tableContentList.get(i) != null) {
                rowContent = tableContentList.get(i);
            }

            if (i == 0) {
                myPaint.setColor(tabHeaderTextColor);
                myPaint.setTextSize(tabHeaderTextSize);
            }
            for (int j = 0; j < cellColCount; j++) {
                if (rowContent.length > j) {
                    canvas.drawText(rowContent[j],
                            cellColLefts[j] + cellColWidths[j] / 2,
                            getTextBaseLine(i * (cellRowH + divW), myPaint),
                            myPaint);
                }
            }
            if (i == 0) {
                // 恢复表格文字画笔样式
                myPaint.setColor(normalTextColor);
                myPaint.setTextSize(normalTextSize);
            }
        }
    }

    /**
     * 计算表格单元格
     */
    private void calculateTableCol() {
        cellColLefts = new float[cellColCount];
        cellColWidths = new float[cellColCount];
        for (int i = 0; i < cellColCount; i++) {
            cellColLefts[i] = getTableColLeft(i);
            cellColWidths[i] = getTableColW(i);
        }
    }

    /**
     * 获取表格单元格列左坐标
     *
     * @param columnIndex 列序列
     * @return 距离
     */
    private float getTableColLeft(int columnIndex) {
        if (cellColWeights == null) {
            return columnIndex * (cellColW + divW);
        }
        // 计算左边的权重和
        int weightSum = 0;
        for (int i = 0; i < columnIndex; i++) {
            if (cellColWeights.length > i) {
                weightSum += cellColWeights[i];
            } else {
                weightSum += 1;
            }
        }
        return columnIndex * divW + weightSum * cellColW;
    }

    /**
     * 获取表格列宽
     *
     * @param columnIndex 列序列
     * @return 宽度
     */
    private float getTableColW(int columnIndex) {
        if (cellColWeights == null) {
            return cellColW;
        }
        int weight = cellColWeights.length > columnIndex ? cellColWeights[columnIndex] : 1;
        return weight * cellColW;
    }

    /**
     * 获取基础行尺寸
     *
     * @param rowStart rowStart
     * @param paint    paint
     * @return 尺寸
     */
    private float getTextBaseLine(float rowStart, Paint paint) {
        final Paint.FontMetrics fontMetrics = paint.getFontMetrics();
        return (rowStart + (rowStart + cellRowH) - fontMetrics.bottom - fontMetrics.top) / 2;
    }

    /**
     * 清除表格数据
     *
     * @return 表格View
     */
    public TableView clearTableContentList() {
        cellColWeights = null;
        tableContentList.clear();
        return this;
    }

    /**
     * 设置表格权重
     *
     * @param cellColWeights 权重数组
     * @return 表格View
     */
    public TableView setTableColWeights(int... cellColWeights) {
        this.cellColWeights = cellColWeights;
        return this;
    }

    /**
     * 设置表格表头
     *
     * @param headers 表头数据
     * @return 表格View
     */
    public TableView setTableHeader(String... headers) {
        tableContentList.add(0, headers);
        return this;
    }

    /**
     * 新增表格内容
     *
     * @param contents 内容数组
     * @return 表格View
     */
    public TableView addTableContent(String... contents) {
        tableContentList.add(contents);
        return this;
    }

    /**
     * 新增表格内容
     *
     * @param contentList 内容列表
     * @return 表格View
     */
    public TableView addTableContent(List<String[]> contentList) {
        tableContentList.addAll(contentList);
        return this;
    }

    /**
     * 初始化表格数据
     */
    private void initTableDataSize() {
        cellRowCount = tableContentList.size();
        if (cellRowCount > 0) {
            // 如果设置了表头，根据表头数量确定列数
            cellColCount = tableContentList.get(0).length;
        }
    }

    /**
     * 刷新表格
     */
    public void refreshTableData() {
        initTableDataSize();
        requestLayout();
        invalidate();
    }
}
