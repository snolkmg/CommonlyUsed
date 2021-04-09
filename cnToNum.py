# -*- coding:utf-8 -*-


class Cn2Arabic():

    # 参考https://blog.csdn.net/hexrain/article/details/52790126
    def __init__(self):
        self.cn_nums = {'〇': 0, '一': 1, '二': 2, '三': 3, '四': 4,
                        '五': 5, '六': 6, '七': 7, '八': 8, '九': 9,
                        '零': 0, '壹': 1, '贰': 2, '叁': 3, '肆': 4,
                        '伍': 5, '陆': 6, '柒': 7, '捌': 8, '玖': 9,
                        '貮': 2, '两': 2, }

        self.uint = {'十': 10, '拾': 10,
                     '百': 100, '佰': 100,
                     '千': 1000, '仟': 1000,
                     '万': 10000, '萬': 10000,
                     '亿': 100000000, '億': 100000000,
                     '兆': 1000000000000, }

    def transform(self, zh_nums):
        nums = []
        unit = 1
        for s in reversed(zh_nums):
            if s in self.cn_nums.keys():
                num = self.cn_nums[s] * unit
                nums.append(num)
            else:
                value = self.uint[s]
                if value in (10000, 100000000):
                    nums.append(value)
                    unit = 1
                else:
                    unit = value

        #print(zh_nums)
        if unit == 10 and zh_nums.startswith('十'):  # 处理'十一'这种情况
            nums.append(10)

        res, tmp = 0, 0
        for num in reversed(nums):
            if num in (10000, 100000000):
                tmp *= num
                res += tmp
                tmp = 0
            else:
                tmp += num
        res += tmp
        return res


if __name__ == "__main__":
    cn_aribaric = Cn2Arabic()
    test_nums = ['八',
                 '十一',
                 '一十八',
                 '二十一',
                 '八十三',
                 '九十六',
                 '一百零一',
                 '一百二十一',
                 '一百二十三',
                 '一千二百零三',
                 '一万一千一百零一',
                 '一十万零三千六百零九',
				 '一十三万三千六百零九',
                 '一百二十三万四千五百六十七',
                 '一千一百二十三万四千五百六十七',
                 '一亿一千一百二十三万四千五百六十七',
                 '一百零二亿五千零一万一千零三十八']

    for item in test_nums:
        print(cn_aribaric.transform(item))
