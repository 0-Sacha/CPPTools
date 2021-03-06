
#pragma once

#include "CPPTools/Formatter/Core/Detail.h"

#include "BaseFormat/FormatType.h"
#include "BaseFormat/NamedArgs.h"
#include "BaseFormat/FormatArgs.h"

namespace CPPTools::Fmt::Detail {

	template <typename... Types>
	struct FormatContextArgsTuple;

    template <>
    struct FormatContextArgsTuple<> {

    public:
        FormatContextArgsTuple() = default;

	public:
		static inline constexpr std::size_t Size() { return 0; }

    public:
        template <typename FormatContext>
        inline void FormatTypeFromIdx(FormatContext &context, FormatIdx idx)        {}

        template <typename FormatContext>
        inline void GetParameterDataFromIdx(FormatContext &context, FormatIdx idx)  {}

        template <typename FormatContext>
        inline void GetParameterData(FormatContext &context, FormatIdx idx)         { context.GetFormatData() = FormatContext::FormatDataType(); }

        template <typename FormatContext>
        inline void GetNamedArgsIdx(FormatContext& context, FormatIdx& idx, FormatIdx currentIdx) { idx = FORMAT_IDX_NOT_FOUND; }

        template <class ValueType>
        inline void GetFormatValueAt(ValueType& value, FormatIdx idx)               {}
    };





    template <typename Type, typename... Rest>
    struct FormatContextArgsTuple<Type, Rest...> : FormatContextArgsTuple<Rest...>
    {
    private:
        using TypeWithoutRef = std::remove_reference_t<Type>;

    public:
        FormatContextArgsTuple(const TypeWithoutRef &t, Rest &&...rest)     : FormatContextArgsTuple<Rest...>(std::forward<Rest>(rest)...), m_Value(t) {}
        FormatContextArgsTuple(const TypeWithoutRef &&t, Rest &&...rest)    : FormatContextArgsTuple<Rest...>(std::forward<Rest>(rest)...), m_Value(t) {}

    private:
        const TypeWithoutRef &m_Value;

    public:
        static inline constexpr std::size_t Size() { return sizeof...(Rest) + 1; }

    public:
        /////---------- FormatTypeFromIdx ----------/////
        template <typename FormatContext>
        inline void FormatTypeFromIdx(FormatContext &context, FormatIdx idx) {
            if (idx == 0)           context.WriteType(m_Value);
            else if (idx > 0)       FormatContextArgsTuple<Rest...>::FormatTypeFromIdx(context, idx - 1);
        }

        /////---------- GetParameterDataFromIdx ----------/////
        template <typename CharFormat, typename CharBuffer, typename ...ContextArgs, class KType = TypeWithoutRef>
        inline auto GetParameterDataFromIdx(BasicFormatContext<CharFormat, CharBuffer, ContextArgs...> &context, FormatIdx idx) -> std::enable_if_t<std::is_same_v<Detail::GetBaseType<KType>, FormatData<CharFormat>>> {
            if (idx == 0)           context.GetFormatData() = m_Value;
            else if (idx > 0)       FormatContextArgsTuple<Rest...>::GetParameterDataFromIdx(context, idx - 1);
        }

        template <typename CharFormat, typename CharBuffer, typename ...ContextArgs, class KType = TypeWithoutRef>
		inline auto GetParameterDataFromIdx(BasicFormatContext<CharFormat, CharBuffer, ContextArgs...>& context, FormatIdx idx) -> std::enable_if_t<!std::is_same_v<Detail::GetBaseType<KType>, FormatData<CharFormat>>> {
            if (idx > 0)      FormatContextArgsTuple<Rest...>::GetParameterDataFromIdx(context, idx - 1);
        }


        /////---------- GetNamedArgsIdx ----------/////
        template<typename FormatContext, class KType = TypeWithoutRef>
        inline auto GetNamedArgsIdx(FormatContext& context, FormatIdx& idx, FormatIdx currentIdx) -> std::enable_if_t<!Detail::IsANamedArgs<Detail::GetBaseType<KType>>::value>{
            FormatContextArgsTuple<Rest...>::GetNamedArgsIdx(context, idx, currentIdx + 1);
        }

        template<typename FormatContext, class KType = TypeWithoutRef>
        inline auto GetNamedArgsIdx(FormatContext& context, FormatIdx& idx, FormatIdx currentIdx) -> std::enable_if_t<Detail::IsANamedArgs<Detail::GetBaseType<KType>>::value>{
            if (context.FormatStr().NextIsANamedArgs(m_Value.GetName()))  idx = currentIdx;
            else                                                    FormatContextArgsTuple<Rest...>::GetNamedArgsIdx(context, idx, currentIdx + 1);
        }

        /////---------- GetFormatValueAt ----------/////
        template <class ValueType, class KType = TypeWithoutRef>
        inline auto GetFormatValueAt(ValueType& value, FormatIdx idx) -> std::enable_if_t<!std::is_convertible_v<KType, ValueType>> {
		    if (idx > 0)	    FormatContextArgsTuple<Rest...>::GetFormatValueAt(value, idx - 1);
        }

        template <class ValueType, class KType = TypeWithoutRef>
        inline auto GetFormatValueAt(ValueType& value, FormatIdx idx) -> std::enable_if_t<std::is_convertible_v<KType, ValueType>> {
		    if (idx == 0)		value = static_cast<ValueType>(m_Value);
		    else if(idx > 0)    FormatContextArgsTuple<Rest...>::GetFormatValueAt(value, idx - 1);
        }
    };
}